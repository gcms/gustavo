package gustavocms.airfares.travelocity

import gustavocms.airfares.Itinerary
import gustavocms.airfares.Price
import gustavocms.airfares.Route

import java.util.regex.Matcher
import java.util.regex.Pattern

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 15:28
 * To change this template use File | Settings | File Templates.
 */
class TravelocityParser {
//    http://travel.travelocity.com/flights/FlightsInboundItineraryService.do?flightType=roundtrip&dateTypeSelect=EXACT_DATES&rid=325910012186592&selectedLeg0=1_26-30&leavingDate=09%2F20%2F2013&leavingFrom=BSB&goingTo=MIA&dateLeavingTime=1200&originalLeavingTime=Anytime&dateReturningTime=1200&originalReturningTime=Anytime&returningDate=10%2F17%2F2013&adults=1&seniors=0&children=0&paxCount=1&classOfService=ECONOMY&fareType=all&filterStops=&airlineSearchPref=&membershipLevel=NO_VALUE&showMeMoreCarrier=
//    http://travel.travelocity.com/flights/FlightsInboundItineraryService.do?flightType=roundtrip&dateTypeSelect=EXACT_DATES&rid=325910012186592&selectedLeg0=7_102  &leavingDate=09%2F20%2F2013&leavingFrom=BSB&goingTo=MIA&dateLeavingTime=1200&originalLeavingTime=Anytime&dateReturningTime=1200&originalReturningTime=Anytime&returningDate=10%2F17%2F2013&adults=1&seniors=0&children=0&paxCount=1&classOfService=ECONOMY&fareType=all&filterStops=&airlineSearchPref=&membershipLevel=NO_VALUE&showMeMoreCarrier=


    private static Route getRoute(Map route) {
        Date departure = new Date(route.departureTime)
        Date arrival = new Date(route.arrivalTime)

        String from = getCode(route.departureLocation)
        String to = getCode(route.arrivalLocation)

        new Route(from, to, departure, arrival)
    }

    Itinerary parseItinerary(Map outbound, Map inbound) {
        Itinerary itinerary = new Itinerary()
        itinerary.price = new Price(new BigDecimal(inbound.totalFare), 'USD')

        itinerary.routes = [ getRoute(outbound), getRoute(inbound) ]

        itinerary
    }

    private static String getCode(String airport) {
        Pattern pattern = ~/.*\(([A-Z]+)\).*/
        Matcher matcher = pattern.matcher(airport)
        if (!matcher.matches())
            throw new RuntimeException()

        matcher.group(1)
    }
}
