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
//        initOutubro()
//        CSVImporter.importFile()

        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        ['BSB', 'GYN', 'RIO', 'SAO'].each { String from ->
            def mia_orl = new FlightQuery()
            mia_orl.routes << new FlightQueryRoute(from, 'MIA', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
            mia_orl.routes << new FlightQueryRoute('ORL', from, format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
            saveQuery(mia_orl)

            def orl_mia = new FlightQuery()
            orl_mia.routes << new FlightQueryRoute(from, 'ORL', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
            orl_mia.routes << new FlightQueryRoute('MIA', from, format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
            saveQuery(orl_mia)
        }

        def zrh_par_dub = new FlightQuery()
        zrh_par_dub.routes << new FlightQueryRoute('GYN', 'ZRH', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        zrh_par_dub.routes << new FlightQueryRoute('DUB', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(zrh_par_dub)

        def dub_par = new FlightQuery()
        dub_par.routes << new FlightQueryRoute('GYN', 'PAR', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        dub_par.routes << new FlightQueryRoute('PAR', 'DUB', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        dub_par.routes << new FlightQueryRoute('DUB', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(dub_par)

        def mex_mia = new FlightQuery()
        mex_mia.routes << new FlightQueryRoute('GYN', 'MEX', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        mex_mia.routes << new FlightQueryRoute('MEX', 'MIA', format.parse('09/02/2014 08:00'), format.parse('12/02/2014 23:59'))
        mex_mia.routes << new FlightQueryRoute('MIA', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(mex_mia)

        def mex_mia2 = FlightQuery.createRoundTripQuery('MEX', 'MIA',
                new DateTimeInterval(format.parse('12/02/2014 11:00'), format.parse('16/02/2014 23:59')),
                new DateTimeInterval(format.parse('20/02/2014 11:00'), format.parse('28/02/2014 23:59')))
        saveQuery(mex_mia2)

        def mex_mia_orl = new FlightQuery()
        mex_mia_orl.routes << new FlightQueryRoute('MEX', 'MIA', new DateTimeInterval(format.parse('12/02/2014 11:00'), format.parse('16/02/2014 23:59')))
        mex_mia_orl.routes << new FlightQueryRoute('ORL', 'MEX', new DateTimeInterval(format.parse('20/02/2014 11:00'), format.parse('28/02/2014 23:59')))
        saveQuery(mex_mia_orl)

        def mex_orl_mia = new FlightQuery()
        mex_orl_mia.routes << new FlightQueryRoute('MEX', 'ORL', new DateTimeInterval(format.parse('12/02/2014 11:00'), format.parse('16/02/2014 23:59')))
        mex_orl_mia.routes << new FlightQueryRoute('MIA', 'MEX', new DateTimeInterval(format.parse('20/02/2014 11:00'), format.parse('28/02/2014 23:59')))
        saveQuery(mex_orl_mia)

        def bog_mia = new FlightQuery()
        bog_mia.routes << new FlightQueryRoute('GYN', 'BOG', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        bog_mia.routes << new FlightQueryRoute('BOG', 'MIA', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        bog_mia.routes << new FlightQueryRoute('MIA', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(bog_mia)

        def lim_sfo = new FlightQuery()
        lim_sfo.routes << new FlightQueryRoute('SAO', 'LIM', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        lim_sfo.routes << new FlightQueryRoute('LIM', 'SFO', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        lim_sfo.routes << new FlightQueryRoute('SFO', 'SAO', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(lim_sfo)

        def mex_pty = new FlightQuery()
        mex_pty.routes << new FlightQueryRoute('GYN', 'PTY', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
//        mex_pty.routes << new FlightQueryRoute('PTY', 'MEX', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        mex_pty.routes << new FlightQueryRoute('MEX', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(mex_pty)

        def mex_cun = new FlightQuery()
        mex_cun.routes << new FlightQueryRoute('GYN', 'MEX', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
//        mex_pty.routes << new FlightQueryRoute('PTY', 'MEX', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        mex_cun.routes << new FlightQueryRoute('CUN', 'GYN', format.parse('01/03/2014 12:00'), format.parse('05/03/2014 23:59'))
        saveQuery(mex_cun)

        def rio_lim = new FlightQuery()
        rio_lim.routes << new FlightQueryRoute('GYN', 'RIO', format.parse('07/02/2014 15:00'), format.parse('08/02/2014 13:40'))
        rio_lim.routes << new FlightQueryRoute('RIO', 'LIM', format.parse('09/02/2014 00:00'), format.parse('12/02/2014 23:59'))
        rio_lim.routes << new FlightQueryRoute('LIM', 'RIO', format.parse('27/02/2014 00:00'), format.parse('02/03/2014 23:59'))
        rio_lim.routes << new FlightQueryRoute('RIO', 'GYN', format.parse('03/03/2014 04:00'), format.parse('05/03/2014 23:59'))
        saveQuery(rio_lim)

        def rio_bog = new FlightQuery()
        rio_bog.routes << new FlightQueryRoute('GYN', 'RIO', format.parse('07/02/2014 15:00'), format.parse('08/02/2014 13:40'))
        rio_bog.routes << new FlightQueryRoute('RIO', 'LIM', format.parse('09/02/2014 00:00'), format.parse('12/02/2014 23:59'))
        rio_bog.routes << new FlightQueryRoute('LIM', 'BOG', format.parse('27/02/2014 00:00'), format.parse('02/03/2014 23:59'))
        rio_bog.routes << new FlightQueryRoute('BOG', 'GYN', format.parse('03/03/2014 04:00'), format.parse('05/03/2014 23:59'))
        saveQuery(rio_bog)

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

        List destinations = ['LIS', 'YQB', 'MIL', 'FRA', 'SLA', 'YEA', 'AKL', 'BOG', 'VIE', 'PAR', 'DUB', 'MIA', 'NYC', 'MEX', 'COR', 'LIM', 'MUC', 'BCN', 'YMQ', 'YYZ', 'CPT', 'LON', 'ROM', 'PRG', 'FLO', 'SYD', 'KRK', 'BEG', 'BUD', 'ZRH', 'SFO', 'LIS', 'SCL', 'ORL', 'UIO']
        destinations.each { String dst ->
            createQuery('GYN', dst, leave, retur)
            createQuery('BSB', dst, leave, retur)
            createQuery('SAO', dst, leave, retur)
            createQuery('RIO', dst, leave, retur)
        }
    }

    private void initOutubro() {
        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        DateTimeInterval leave = new DateTimeInterval(format.parse('23/10/2013 14:00'), format.parse('24/10/2013 10:00'))
        DateTimeInterval retur = new DateTimeInterval(format.parse('28/10/2013 16:00'), format.parse('29/10/2013 12:30'))

        ['BEL', 'MAO', 'REC', 'FOR', 'RIO', 'MCZ', 'AJU', 'NAT'].each { String dst ->
            createQuery('GYN', dst, leave, retur)
        }
    }

    private static void createQuery(String src, String dst, DateTimeInterval leave, DateTimeInterval retur) {
        if (Query.findAllByContentLike("${src};${dst};%${dst};${src};%"))
            return

        def fq = FlightQuery.createRoundTripQuery(src, dst, leave, retur)

        def q = new Query(flightQuery: fq)
        q.save()
        println q
    }

    def destroy = {
    }
}
