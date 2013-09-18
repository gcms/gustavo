package gustavocms.airfares.query
/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 10/09/13
 * Time: 16:31
 * To change this template use File | Settings | File Templates.
 */
class FlightQueryRoute {
    String from
    String to

    DateTimeInterval departureInterval

    public FlightQueryRoute(String from, String to, Date departureStart, Date departureEnd) {
        this(from, to, new DateTimeInterval(departureStart, departureEnd))
    }

    public FlightQueryRoute(String from, String to, Date departureDay) {
        this(from, to, DateTimeInterval.getIntervalFullDay(departureDay))
    }

    public FlightQueryRoute(String from, String to, DateTimeInterval departureInterval) {
        this.from = from
        this.to = to
        this.departureInterval = departureInterval
    }

    String toString() {
        "${from}-${to} (${departureInterval})"
    }
}

