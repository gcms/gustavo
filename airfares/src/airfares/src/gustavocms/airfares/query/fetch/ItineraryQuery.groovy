package gustavocms.airfares.query.fetch

import gustavocms.airfares.query.FlightQueryRoute

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 11/09/13
 * Time: 08:19
 * To change this template use File | Settings | File Templates.
 */
class ItineraryQuery {

    List<ItineraryQueryRoute> routes = []

    String toString() {
        routes.join(', ')
    }

    boolean isOneWay() {
        type == ItineraryQueryType.ONE_WAY
    }

    boolean isRoundTrip() {
        type == ItineraryQueryType.ROUND_TRIP
    }

    boolean isMultiDest() {
        type == ItineraryQueryType.MULTI_DEST
    }

    ItineraryQueryType getType() {
        if (routes.size() == 1)
            return ItineraryQueryType.ONE_WAY

        if (routes.size() == 2 && isRoundTrip(routes.first(), routes.last()))
            return ItineraryQueryType.ROUND_TRIP

        return ItineraryQueryType.MULTI_DEST
    }

    static boolean isRoundTrip(ItineraryQueryRoute leave, ItineraryQueryRoute retur) {
        leave.from == retur.to && leave.to == retur.from
    }
}
