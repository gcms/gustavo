package gustavocms.airfares

import gustavocms.airfares.despegar.DecolarProvider

import java.util.concurrent.CopyOnWriteArrayList

class QueryService {
    List<Query> queries = new CopyOnWriteArrayList<Query>()

    List<BestPrice> getBestPricesForWeek(Query query) {
        queries.add(query)
//        checkPrice(query)

        Calendar today = new Date().toCalendar()
//        today.setFirstDayOfWeek(Calendar.SUNDAY)
//        today.set(Calendar.DAY_OF_WEEK, Calendar.SUNDAY)

        BestPrice.executeQuery("from BestPrice where query.id = :query and day between :inicio and :fim order by day desc", [query: query.id, inicio: today.time - 14, fim: today.time])
//        BestPrice.executeQuery("from BestPrice where query.id = :query", [query: query.id])
    }

    List<Deal> getBestDeals() {
        Date yesterday = new Date().clearTime() - 1
        getBestDeals(yesterday - 7, yesterday)
    }

    List<Deal> getPriceDrops() {
        Date yesterday = new Date().clearTime() - 1
        getBestDeals(yesterday, yesterday)
    }

    List<Deal> getAllTimePriceDrops() {
        Date yesterday = new Date().clearTime() - 1
        getBestDeals(yesterday - 365, yesterday, 'min')
    }


    List<Deal> getBestDeals(Date start, Date end, String func = 'avg') {
        Date today = new Date().clearTime()

        Query.executeQuery("""
select q, ${func}(bpOthers.price), (1 - (bpToday.price / ${func}(bpOthers.price))) as discount
from Query q, BestPrice bpToday, BestPrice bpOthers
where q.id = bpToday.query.id and bpToday.day = :today
and q.id = bpOthers.query.id and bpOthers.day <> :today and bpOthers.day between :start and :end
group by q, q.content, q.version, bpToday, bpToday.price, bpToday.version
having bpToday.price <= ${func}(bpOthers.price)
""", [today: today, start: start.clone().clearTime(), end: end.clone().clearTime()]).
        sort { -it[2] }.
        collect {
            new Deal(query: it[0], average: it[1])
        }
    }

    void checkPrice(Query query) {
        println "Checking... $query"
        Date today = new Date().clearTime()
        if (!BestPrice.findByQueryAndDay(query, today)) {
            print "Not found ${query.id} ${today}"
            Price p = getBestPrice(query)
            if (p == null)
                return

            BestPrice bp = new BestPrice(query: query, day: today, price: p.getPrice('USD'), currency: 'USD' )
            bp.save()
            bp.errors.allErrors.each { println it }
        }
    }

    private Price getBestPrice(Query query) {
        try {
            return new DecolarProvider().getBestPrice(query.flightQuery)
        } catch (Exception ignored) {
            return null
        }
    }

    Itinerary getBestItinerary(Query query) {
        List<Itinerary> itineraries = new DecolarProvider().getBestItineraries(query.flightQuery)
        itineraries.empty ? null : itineraries.first()
    }

}
