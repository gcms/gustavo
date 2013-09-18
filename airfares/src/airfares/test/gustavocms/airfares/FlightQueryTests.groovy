package gustavocms.airfares

import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.FlightQueryRoute
import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.query.fetch.ItineraryQueryRoute

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 11/09/13
 * Time: 08:56
 * To change this template use File | Settings | File Templates.
 */
class FlightQueryTests extends GroovyTestCase {
    void test2LegQuery() {
        DateFormat df = new SimpleDateFormat('dd/MM/yyyy')
        def fq = new FlightQuery()
        fq.routes << new FlightQueryRoute('GYN', 'BUE', df.parse('19/11/2013'), df.parse('21/11/2013'))
        fq.routes << new FlightQueryRoute('MVD', 'GYN', df.parse('01/12/2013'), df.parse('02/12/2013'))

        List<ItineraryQuery> queries = fq.getQueries()
        queries.each { println it }


        assertEquals(3*(2+1), queries.size())

        ItineraryQuery first = queries.first()
        ItineraryQueryRoute gynBue = first.routes.first()
        assertEquals 'GYN', gynBue.from
        assertEquals 'BUE', gynBue.to
        assertEquals '19/11/2013', df.format(gynBue.day)
    }

    void test3LegQuery() {
        DateFormat df = new SimpleDateFormat('dd/MM/yyyy')
        def fq = new FlightQuery()
        fq.routes << new FlightQueryRoute('GYN', 'BUE', df.parse('19/11/2013'))
        fq.routes << new FlightQueryRoute('BUE', 'SCL', df.parse('01/12/2013'), df.parse('02/12/2013'))
        fq.routes << new FlightQueryRoute('SCL', 'GYN', df.parse('11/12/2013'), df.parse('12/12/2013'))

        List<ItineraryQuery> queries = fq.getQueries()
        queries.each { println it }

        assertEquals(1*3*3, queries.size())

        ItineraryQuery first = queries.first()
        ItineraryQueryRoute gynBue = first.routes.first()
        assertEquals 'GYN', gynBue.from
        assertEquals 'BUE', gynBue.to
        assertEquals '19/11/2013', df.format(gynBue.day)
    }
}
