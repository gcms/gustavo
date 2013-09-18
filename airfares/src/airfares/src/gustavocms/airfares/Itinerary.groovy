package gustavocms.airfares

import org.apache.commons.lang3.time.DateFormatUtils

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 13:17
 * To change this template use File | Settings | File Templates.
 */
class Itinerary {
    List<Route> routes
    Price price

    public int hashCode() {
        routes.hashCode() + price.hashCode()
    }

    public boolean equals(Itinerary itinerary) {
        routes.equals(itinerary.routes) && price.equals(itinerary.price)
    }

    public long getTotalDuration() {
        routes.sum { it.duration }
    }

    public int getNumDays() {
        routes.last().departure - routes.first().arrival
    }

    String toString() {
        if (routes.size() == 2)
            return toRoundTripString()

        String from = routes*.from.join(',')
        String to = routes*.to.join(',')

        DateFormat df = new SimpleDateFormat('dd/MM/yyyy')
        String dateTimes = routes.collect { df.format(it.departure) }

        "${price} ${from}/${to}/${dateTimes}"
    }

    String toRoundTripString() {
        "${price} ${routes.first()} / ${routes.last()} [${DateFormatUtils.format(getTotalDuration(), "HH:mm")}]"
    }
}
