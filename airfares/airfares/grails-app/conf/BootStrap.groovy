import gustavocms.airfares.FareLoader
import gustavocms.airfares.Query
import gustavocms.airfares.QueryService
import gustavocms.airfares.query.DateTimeInterval
import gustavocms.airfares.query.FlightQuery

import java.text.DateFormat
import java.text.SimpleDateFormat

class BootStrap {
    QueryService queryService

    def init = { servletContext ->
        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        DateTimeInterval leave = new DateTimeInterval(format.parse('23/10/2013 15:00'), format.parse('24/10/2013 09:00'))
        DateTimeInterval retur = new DateTimeInterval(format.parse('27/10/2013 12:00'), format.parse('28/10/2013 12:00'))

        createQuery('GYN', 'BSB', leave, retur)
        createQuery('GYN', 'SAO', leave, retur)

        List destinations = ['YEA', 'AKL', 'BOG', 'VIE', 'PAR', 'DUB', 'MIA', 'NYC', 'MEX', 'COR', 'LIM', 'MUC', 'BCN', 'YMQ', 'YYZ', 'CPT', 'LON', 'ROM', 'PRG', 'FLO', 'SYD', 'KRK', 'BEG', 'BUD', 'ZRH', 'SFO', 'LIS', 'SCL', 'ORL']
        destinations.each { String dst ->
            createQuery('GYN', dst, leave, retur)
            createQuery('BSB', dst, leave, retur)
            createQuery('SAO', dst, leave, retur)
        }

        new FareLoader(queryService).start()
    }

    private static void createQuery(String src, String dst, DateTimeInterval leave, DateTimeInterval retur) {
        def fq = FlightQuery.createRoundTripQuery(src, dst, leave, retur)
        def q = new Query(flightQuery: fq)
        q.save(failOnError: true)
        println q
    }

    def destroy = {
    }
}
