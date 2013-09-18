package gustavocms.airfares.query

import gustavocms.airfares.Itinerary
import gustavocms.airfares.Route
import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.query.fetch.ItineraryQueryRoute

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 12:24
 * To change this template use File | Settings | File Templates.
 */
class FlightQuery {
    List<FlightQueryRoute> routes = []

    static FlightQuery createOneWay(String from, String to, Date date) {
        def fqRoute = new FlightQueryRoute(from, to, date)
        new FlightQuery(routes: [fqRoute])
    }

    List<ItineraryQuery> getQueries() {
        List<ItineraryQuery> result = []
        createQueries(result, [], routes)
        result
    }

    private void createQueries(List<ItineraryQuery> result, List<ItineraryQueryRoute> previousRoutes, List<FlightQueryRoute> routes) {
        FlightQueryRoute first = routes.first()
        List<FlightQueryRoute> others = routes.size() == 1 ? [] : routes[1..-1]


        int stepStart = previousRoutes.empty ? 0 : -1

        first.departureInterval.getDays(stepStart).each { Date day ->
            List<ItineraryQueryRoute> currentRoutes = []
            currentRoutes.addAll(previousRoutes)
            currentRoutes << new ItineraryQueryRoute(from: first.from, to: first.to, day: day)

            if (others.empty)
                result << new ItineraryQuery(routes: currentRoutes)
            else
                createQueries(result, currentRoutes, others)
        }
    }

    public boolean isRoundTrip() {
        routes.size() == 2 && isRoundTrip(routes.first(), routes.last())
    }

    public boolean isOneWay() {
        routes.size() == 1
    }

    public boolean isMultiDest() {
        !isOneWay() && !isRoundTrip()
    }

    public static boolean isRoundTrip(FlightQueryRoute leave, FlightQueryRoute retur) {
        leave.from == retur.to && leave.to == retur.from
    }

    static FlightQuery createRoundTripQuery(String from, String to, DateTimeInterval leave, DateTimeInterval retur) {
        FlightQueryRoute fqLeave = new FlightQueryRoute(from, to, leave)
        FlightQueryRoute fqRetur = new FlightQueryRoute(to, from, retur)

        new FlightQuery(routes: [fqLeave, fqRetur])
    }

    boolean filter(Itinerary itinerary) {
        if (itinerary.routes.size() != routes.size())
            return false

        if (isRoundTrip())
            return filterRoundTrip(itinerary)

        int i = 0
        itinerary.routes.every { Route route ->
            filter(route, routes[i++])
        }
    }

    static boolean filter(Route route, FlightQueryRoute fqRoute) {
        fqRoute.departureInterval.contains(route.departure)
    }

    boolean filterRoundTrip(Itinerary itinerary) {
        assert itinerary.routes.size() == 2

        Route leave = itinerary.routes.first()
        Route retur = itinerary.routes.last()

        FlightQueryRoute fqLeave = routes.first()
        FlightQueryRoute fqRetur = routes.last()

//        println "${fqLeave.departureInterval} contains (${leave.departure.format('dd/MM/yyyy HH:mm')}) && ${fqRetur.departureInterval} contains (${retur.arrival.format('dd/MM/yyyy HH:mm')})"

        fqLeave.departureInterval.contains(leave.departure) &&
                fqRetur.departureInterval.contains(retur.arrival)
    }

    String toString() {
        routes.collect { it.toString() }.join('\n')
    }
}
