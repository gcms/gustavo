package gustavocms.airfares

import gustavocms.airfares.decolar.DecolarProvider

import java.util.concurrent.CopyOnWriteArrayList

class QueryService {
    List<Query> queries = new CopyOnWriteArrayList<Query>()

    List<BestPrice> getBestPricesForWeek(Query query) {
        queries.add(query)
//        checkPrice(query)

        Calendar today = new Date().toCalendar()
        today.setFirstDayOfWeek(Calendar.SUNDAY)
        today.set(Calendar.DAY_OF_WEEK, Calendar.SUNDAY)

        BestPrice.executeQuery("from BestPrice where query.id = :query and day between :inicio and :fim", [query: query.id, inicio: today.time, fim: today.time + 7])
//        BestPrice.executeQuery("from BestPrice where query.id = :query", [query: query.id])
    }

    void checkPrice(Query query) {
        println "Checking... $query"
        Date today = new Date().clearTime()
        if (!BestPrice.findByQueryAndDay(query, today)) {
            print "Not found ${query.id} ${today}"
            Price p = new DecolarProvider().getBestPrice(query.flightQuery)
            BestPrice bp = new BestPrice(query: query, day: today, price: p.getPrice('USD'), currency: 'USD' )
            bp.save()
            bp.errors.allErrors.each { println it }
        }
    }

    Itinerary getBestItinerary(Query query) {
        List<Itinerary> itineraries = new DecolarProvider().getBestItineraries(query.flightQuery)
        itineraries.empty ? null : itineraries.first()
    }
}
