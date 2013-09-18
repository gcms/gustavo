package gustavocms.airfares.travelocity

import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.query.fetch.ItineraryQueryRoute

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 16/09/13
 * Time: 12:06
 * To change this template use File | Settings | File Templates.
 */
class TravelocityURLBuilder {
    public URL buildRoundTripURL(ItineraryQuery query) {
        DateFormat df = new SimpleDateFormat('MM/dd/yyyy')

        ItineraryQueryRoute leave = query.routes.first()
        ItineraryQueryRoute retur = query.routes.last()

        String from = leave.from.toUpperCase()
        String to = leave.to.toUpperCase()

        String leaveDay = df.format(leave.day).replace('/', '%2F')
        String returDay = df.format(retur.day).replace('/', '%2F')

        new URL("http://travel.travelocity.com/flights/FlightsItineraryService.do?flightType=roundtrip&dateTypeSelect=EXACT_DATES&leavingDate=${leaveDay}&leavingFrom=${from}&goingTo=${to}&dateLeavingTime=1200&originalLeavingTime=Anytime&dateReturningTime=1200&originalReturningTime=Anytime&returningDate=${returDay}&adults=1&seniors=0&children=0&paxCount=1&classOfService=ECONOMY&fareType=all&filterStops=&airlineSearchPref=&membershipLevel=NO_VALUE")
    }

    public URL buildInboundURLQuery(ItineraryQuery query, String rid, String uniqueId) {
        DateFormat df = new SimpleDateFormat('MM/dd/yyyy')

        ItineraryQueryRoute leave = query.routes.first()
        ItineraryQueryRoute retur = query.routes.last()

        String from = leave.from.toUpperCase()
        String to = leave.to.toUpperCase()

        String leaveDay = df.format(leave.day).replace('/', '%2F')
        String returDay = df.format(retur.day).replace('/', '%2F')


        new URL("http://travel.travelocity.com/flights/FlightsInboundItineraryService.do?flightType=roundtrip&dateTypeSelect=EXACT_DATES&rid=${rid}&selectedLeg0=${uniqueId}&leavingDate=${leaveDay}&leavingFrom=${from}&goingTo=${to}&dateLeavingTime=1200&originalLeavingTime=Anytime&dateReturningTime=1200&originalReturningTime=Anytime&returningDate=${returDay}&adults=1&seniors=0&children=0&paxCount=1&classOfService=ECONOMY&fareType=all&filterStops=&airlineSearchPref=&membershipLevel=NO_VALUE&showMeMoreCarrier=")
//    http://travel.travelocity.com/flights/FlightsInboundItineraryService.do?flightType=roundtrip&dateTypeSelect=EXACT_DATES&rid=325910012186592&selectedLeg0=7_102  &leavingDate=09%2F20%2F2013&leavingFrom=BSB&goingTo=MIA&dateLeavingTime=1200&originalLeavingTime=Anytime&dateReturningTime=1200&originalReturningTime=Anytime&returningDate=10%2F17%2F2013&adults=1&seniors=0&children=0&paxCount=1&classOfService=ECONOMY&fareType=all&filterStops=&airlineSearchPref=&membershipLevel=NO_VALUE&showMeMoreCarrier=

    }
}
