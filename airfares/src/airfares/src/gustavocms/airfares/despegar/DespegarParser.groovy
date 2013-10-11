package gustavocms.airfares.despegar

import gustavocms.airfares.Itinerary
import gustavocms.airfares.Price
import gustavocms.airfares.Route

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 15:28
 * To change this template use File | Settings | File Templates.
 */
class DespegarParser {

    public List<Itinerary> parseMultDestMap(Map json) {
        json?.result?.data?.items.collect { Map item ->
            Price price = new Price(new BigDecimal(item.emissionPrice.total.fare.raw), item.emissionPrice.currencyCode)

            List<Route> routes = item.itinerary.routes.collect { Map route ->
                String from = route.segments.first().departure.location.code
                String to = route.segments.last().arrival.location.code
                Date departureTime = new Date(route.departureDateTime.raw)
                Date arrivalTime = new Date(route.arrivalDateTime.raw)
                new Route(from, to, departureTime, arrivalTime)
            }

            new Itinerary(price: price, routes: routes)
        }

    }

    public  List<Itinerary> parseRoundTripMap(Map json) {
        json?.result?.data?.items.collect { Map item ->
            assert item.itinerariesBox.itinerariesBoxPriceInfoList.size() == 1

            BigDecimal amount = new BigDecimal(item.itinerariesBox.itinerariesBoxPriceInfoList.first().total.fare.raw)
            String currency = item.itinerariesBox.itinerariesBoxPriceInfoList.first().total.fare.formatted.code
            Price price = new Price(amount, currency)

            getCombinedItineraries(price, item.itinerariesBox.outboundRoutes, item.itinerariesBox.inboundRoutes)

        }.flatten().sort { it.getPrice().getPrice('USD') }
    }

    public List<Itinerary> parseOneWayMap(Map json) {
        json?.result?.data?.items.collect { Map item ->
            Map box = item.itinerariesBox
            Map emissionPrice = box.emissionPrice

            Price price = new Price(new BigDecimal(emissionPrice.total.fare.raw), emissionPrice.currencyCode)
            Route route = getRoute(box.outboundRoutes.first())
            new Itinerary(price: price, routes: [route])
        }
    }

    private static List<Itinerary> getCombinedItineraries(Price price, List outboundRoutes, List inboundRoutes) {
        List<Itinerary> itineraries = []

        outboundRoutes.each { outbound ->
            inboundRoutes.each { inbound ->
                List<Route> flights = [ getRoute(outbound), getRoute(inbound) ]
                itineraries << new Itinerary(price: price, routes: flights)
            }
        }

        itineraries
    }

    private static Route getRoute(Map route) {
        Date departure = new Date(route.departureDateTime.raw)
        Date arrival = new Date(route.arrivalDateTime.raw)
        String from = route.segments.first().departure.location.code
        String to = route.segments.last().arrival.location.code

        new Route(from, to, departure, arrival)
    }
}
