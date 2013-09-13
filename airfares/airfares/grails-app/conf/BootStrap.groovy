import gustavocms.airfares.CSVImporter
import gustavocms.airfares.CurrencyUpdater
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
        initDB()
//        CSVImporter.importFile()

        new FareLoader(queryService).start()
        new CurrencyUpdater().start()
    }

    private void initDB() {
        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        DateTimeInterval leave = new DateTimeInterval(format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        DateTimeInterval retur = new DateTimeInterval(format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))

        createQuery('GYN', 'BSB', leave, retur)
        createQuery('GYN', 'SAO', leave, retur)

        List destinations = ['SLA', 'YEA', 'AKL', 'BOG', 'VIE', 'PAR', 'DUB', 'MIA', 'NYC', 'MEX', 'COR', 'LIM', 'MUC', 'BCN', 'YMQ', 'YYZ', 'CPT', 'LON', 'ROM', 'PRG', 'FLO', 'SYD', 'KRK', 'BEG', 'BUD', 'ZRH', 'SFO', 'LIS', 'SCL', 'ORL']
        destinations.each { String dst ->
            createQuery('GYN', dst, leave, retur)
            createQuery('BSB', dst, leave, retur)
            createQuery('SAO', dst, leave, retur)
        }
    }

    private static void createQuery(String src, String dst, DateTimeInterval leave, DateTimeInterval retur) {
        if (Query.findAllByContentLike("${src};${dst}%"))
            return

        def fq = FlightQuery.createRoundTripQuery(src, dst, leave, retur)
        def q = new Query(flightQuery: fq)
        q.save(failOnError: true)
        println q
    }

    def destroy = {
    }
}
