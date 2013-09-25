package gustavocms.airfares

import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.FlightQueryRoute

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 12/09/13
 * Time: 11:26
 * To change this template use File | Settings | File Templates.
 */
class Query {
    public static final LINE_SEPARATOR = '\n'

    String content

    static constraints = {
        content nullable: false, blank: false, unique: true
    }

    static transients = [ 'flightQueryCache', 'flightQuery', 'currentPrice']

    private FlightQuery flightQueryCache

    void setFlightQuery(FlightQuery  fq) {
        this.flightQueryCache = fq
        DateFormat df = new SimpleDateFormat('dd/MM/yyyy HH:mm')
        StringBuilder sb = new StringBuilder()
        fq.routes.each { FlightQueryRoute fqr ->
            sb.append("${fqr.from};${fqr.to};${df.format(fqr.departureInterval.start)};${df.format(fqr.departureInterval.end)}${LINE_SEPARATOR}")
        }
        content = sb.toString()
    }

    FlightQuery getFlightQuery() {
        if (content == null || content.empty)
            return null

        if (flightQueryCache != null)
            return flightQueryCache

        DateFormat df = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        List routes = []
        content.tokenize(LINE_SEPARATOR).each {
            List values = it.tokenize(';')
            routes << new FlightQueryRoute(values[0], values[1], df.parse(values[2]), df.parse(values[3]))
        }

        flightQueryCache = new FlightQuery(routes: routes)
    }

    String toString() {
        if (!flightQuery)
            return 'null'

        DateFormat df = new SimpleDateFormat('dd/MM/yyyy')
        Date first = flightQuery.routes.first().departureInterval.start
        Date last = flightQuery.routes.last().departureInterval.end

        if (flightQuery.roundTrip) {
            return "${flightQuery.routes.first().from}-${flightQuery.routes.first().to} " +
                    "${df.format(first)} - ${df.format(last)}"
        }

        List destinations = []
        flightQuery.routes.each {
            destinations << it.from
            destinations << it.to
        }
        "${destinations.unique().join('-')} ${df.format(first)} - ${df.format(last)}"
    }

    BestPrice getCurrentPrice() {
        BestPrice.findByQueryAndDay(this, new Date().clearTime())
    }
}
