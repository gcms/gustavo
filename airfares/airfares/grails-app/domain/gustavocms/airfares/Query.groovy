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
    public static final LINE_SEPARATOR = System.getProperty('line.separator')

    String content

    static constraints = {
        content nullable: false, blank: false
    }

    static transients = [ 'flightQuery']

    void setFlightQuery(FlightQuery  fq) {
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

        DateFormat df = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        FlightQuery fq = new FlightQuery()
        content.tokenize(LINE_SEPARATOR).each {
            List values = it.tokenize(';')
            fq.routes << new FlightQueryRoute(values[0], values[1], df.parse(values[2]), df.parse(values[3]))
        }
        fq
    }

    String toString() {
        if (!flightQuery)
            return 'null'

        DateFormat df = new SimpleDateFormat('dd/MM/yyyy')
        if (flightQuery.roundTrip) {
            return "${flightQuery.routes.first().from}-${flightQuery.routes.first().to} " +
                    "${df.format(flightQuery.routes.first().departureInterval.start)} - ${df.format(flightQuery.routes.last().departureInterval.end)}"
        }

        flightQuery.toString()
    }
}
