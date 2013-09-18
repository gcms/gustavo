package gustavocms.airfares

import org.apache.commons.lang3.time.DateUtils

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 13:17
 * To change this template use File | Settings | File Templates.
 */
class Route {
    String from
    String to

    Date departure
    Date arrival

    public Route(String from, String to, Date departure, Date arrival) {
        this.from = from
        this.to = to
        this.departure = departure
        this.arrival = arrival
    }

    public int hashCode() {
        from.hashCode() + to.hashCode() + departure.hashCode() + arrival.hashCode()
    }

    public boolean equals(Object other) {
        if (!(other instanceof Route))
            return false

        Route flight = other
        from.equals(flight.from) && to.equals(flight.to) &&
                departure.equals(flight.departure) && arrival.equals(flight.arrival)
    }

    public String toString() {
        "${from} ${departure.format('dd/MM/yyyy HH:mm')} - ${to}${DateUtils.isSameDay(departure, arrival) ? '' : ' +' + (arrival - departure) } ${arrival.format('HH:mm')}"
    }

    public long getDuration() {
        arrival.time - departure.time
    }
}
