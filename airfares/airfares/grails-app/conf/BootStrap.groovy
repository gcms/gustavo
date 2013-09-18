import gustavocms.airfares.BestPrice
import gustavocms.airfares.CurrencyUpdater
import gustavocms.airfares.FareLoader
import gustavocms.airfares.Query
import gustavocms.airfares.QueryService
import gustavocms.airfares.query.DateTimeInterval
import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.FlightQueryRoute

import java.text.DateFormat
import java.text.SimpleDateFormat

class BootStrap {
    QueryService queryService

    def init = { servletContext ->
        initDB()
//        CSVImporter.importFile()

        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        def dub_par = new FlightQuery()
        dub_par.routes << new FlightQueryRoute('GYN', 'PAR', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        dub_par.routes << new FlightQueryRoute('PAR', 'DUB', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        dub_par.routes << new FlightQueryRoute('DUB', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(dub_par)

        def rio_lim = new FlightQuery()
        rio_lim.routes << new FlightQueryRoute('GYN', 'RIO', format.parse('07/02/2014 15:00'), format.parse('08/02/2014 13:40'))
        rio_lim.routes << new FlightQueryRoute('RIO', 'LIM', format.parse('09/02/2014 00:00'), format.parse('12/02/2014 23:59'))
        rio_lim.routes << new FlightQueryRoute('LIM', 'RIO', format.parse('27/02/2014 00:00'), format.parse('02/03/2014 23:59'))
        rio_lim.routes << new FlightQueryRoute('RIO', 'GYN', format.parse('03/03/2014 04:00'), format.parse('05/03/2014 23:59'))
        saveQuery(rio_lim)

        def rio_par = new FlightQuery()
        rio_par.routes << new FlightQueryRoute('GYN', 'RIO', format.parse('07/02/2014 15:00'), format.parse('08/02/2014 13:40'))
        rio_par.routes << new FlightQueryRoute('RIO', 'PAR', format.parse('09/02/2014 00:00'), format.parse('12/02/2014 23:59'))
        rio_par.routes << new FlightQueryRoute('PAR', 'RIO', format.parse('27/02/2014 00:00'), format.parse('02/03/2014 23:59'))
        rio_par.routes << new FlightQueryRoute('RIO', 'GYN', format.parse('03/03/2014 04:00'), format.parse('05/03/2014 23:59'))
        saveQuery(rio_par)




        new FareLoader(queryService).start()
        new CurrencyUpdater().start()
    }

    void saveQuery(FlightQuery fq) {
        def q  = new Query(flightQuery: fq)
        if (!Query.findByContent(q.content))
            q.save()
    }

    private void initDB() {
        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        DateTimeInterval leave = new DateTimeInterval(format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        DateTimeInterval retur = new DateTimeInterval(format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))

        createQuery('GYN', 'BSB', leave, retur)
        createQuery('GYN', 'SAO', leave, retur)
        createQuery('GYN', 'RIO', leave, retur)

        List destinations = ['YQB', 'MIL', 'FRA', 'SLA', 'YEA', 'AKL', 'BOG', 'VIE', 'PAR', 'DUB', 'MIA', 'NYC', 'MEX', 'COR', 'LIM', 'MUC', 'BCN', 'YMQ', 'YYZ', 'CPT', 'LON', 'ROM', 'PRG', 'FLO', 'SYD', 'KRK', 'BEG', 'BUD', 'ZRH', 'SFO', 'LIS', 'SCL', 'ORL']
        destinations.each { String dst ->
            createQuery('GYN', dst, leave, retur)
            createQuery('BSB', dst, leave, retur)
            createQuery('SAO', dst, leave, retur)
            createQuery('RIO', dst, leave, retur)
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
