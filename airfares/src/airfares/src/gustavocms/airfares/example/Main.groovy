package gustavocms.airfares.example

import gustavocms.airfares.Itinerary
import gustavocms.airfares.Price
import gustavocms.airfares.Provider
import gustavocms.airfares.cache.Cache
import gustavocms.airfares.despegar.DespegarProvider
import gustavocms.airfares.query.DateTimeInterval
import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.FlightQueryRoute
import gustavocms.airfares.travelocity.TravelocityProvider

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 02/09/13
 * Time: 21:44
 * To change this template use File | Settings | File Templates.
 */
class Main {

    private DBWriter dbWriter
    private Provider travelocity
    private Provider decolar

    public Main() {
        dbWriter = new DBWriter()
        travelocity = new TravelocityProvider()
        decolar = new DespegarProvider()
    }

    static void main(String[] args) {
        Locale.default = new Locale('en', 'US')

        try {
            new Main().execute()
        } finally {
//            CacheManager.getInstance().shutdown()
        }
    }

    public void execute() {
        executeMultiDest()

        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')

        Date departureStart = format.parse('07/02/2014 15:00')
        Date departureEnd = format.parse('08/02/2014 23:59')

        Date arrivalStart = format.parse('01/03/2014 15:00')
        Date arrivalEnd = format.parse('05/03/2014 23:59')

        List destinations = ['YEA', 'AKL', 'BOG', 'VIE', 'PAR', 'DUB', 'MIA', 'NYC', 'MEX', 'COR', 'LIM', 'MUC', 'BCN', 'YMQ', 'YYZ', 'CPT', 'LON', 'ROM', 'PRG', 'FLO', 'SYD', 'KRK', 'BEG', 'BUD', 'ZRH', 'SFO', 'LIS', 'SCL', 'ORL']

//        List origins = [ 'GYN' ]
//        List destinations = ['PRG']

        List result = []

        result.add(executeQuery('GYN', 'BSB', departureStart, departureEnd, arrivalStart, arrivalEnd))
        result.add(executeQuery('GYN', 'SAO', departureStart, departureEnd, arrivalStart, arrivalEnd))
        destinations.each { String dest ->
            result.add(executeQuery('GYN', dest, departureStart, departureEnd, arrivalStart, arrivalEnd))
            result.add(executeQuery('BSB', dest, (departureStart + 1).clearTime(), departureEnd + 1, arrivalStart - 1 , arrivalEnd - 1))
            result.add(executeQuery('SAO', dest, (departureStart + 1).clearTime(), departureEnd + 1, arrivalStart - 1 , arrivalEnd - 1))
        }

        DateTimeInterval leave = new DateTimeInterval(format.parse('23/10/2013 15:00'), format.parse('24/10/2013 10:00'))
        DateTimeInterval retur = new DateTimeInterval(format.parse('27/10/2013 10:00'), format.parse('28/10/2013 12:00'))
        ['BEL', 'MAO', 'REC', 'FOR', 'RIO', 'MCZ', 'AJU', 'NAT'].each { String dst ->
            executeQuery('GYN', dst, leave.start, leave.end, retur.start, retur.end)
        }

        StringBuilder sb = new StringBuilder("src;dest;price;leave;return\n")
        result.sort { it.dest + it.src }.each { map ->
            getCsvLine(sb, map)
        }
        File dir = new File('results')
        if (!dir.exists())
            dir.mkdir()

        File arquivo = new File(dir, new Date().format('yyyy-MM-dd') + '.csv')
        arquivo.text = sb.toString()
    }

    void executeMultiDest() {
        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')
        def fq = new FlightQuery()
        fq.routes << new FlightQueryRoute('GYN', 'PAR', format.parse('07/02/2014 15:00'), format.parse('09/02/2014 23:59'))
        fq.routes << new FlightQueryRoute('PAR', 'DUB', format.parse('16/02/2014 08:00'), format.parse('19/02/2014 23:59'))
        fq.routes << new FlightQueryRoute('DUB', 'GYN', format.parse('01/03/2014 12:00'), format.parse('04/03/2014 23:59'))

        decolar.getBestItineraries(fq).each {
            println it
        }
    }

    private Map executeQuery(String src, String dest, Date departureStart, Date departureEnd, Date arrivalStart, Date arrivalEnd) {
//        def rtfq = new RoundTripFlightQuery(src, dest, departureStart, departureEnd, arrivalStart, arrivalEnd)
        def rtfq = FlightQuery.createRoundTripQuery(src, dest, new DateTimeInterval(departureStart, departureEnd), new DateTimeInterval(arrivalStart, arrivalEnd))
//                Price price = travelocity.getBestPrice(rtfq)

//        [ executeWithProvider(travelocity, rtfq, src, dest), executeWithProvider(despegar, rtfq, src, dest) ].last()
        executeWithProvider(decolar, rtfq, src, dest)
    }

    private LinkedHashMap<String, Object> executeWithProvider(Provider provider, FlightQuery rtfq, String src, String dest) {
        List itineraries = provider.getBestItineraries(rtfq)
        if (itineraries.empty)
            return null

        Price price = itineraries.first().price

        Cache.DEFAULT.flush()


        Itinerary best = itineraries.first()

        println "${src}-${dest} ${price} (${price.getPrice('BRL')}) [$best]"
        dbWriter.write(new Date().clearTime(), price, best.routes.first(), best.routes.last())

        [src: src, dest: dest, price: price, itineraries: itineraries]
    }

    private static void getCsvLine(StringBuilder sb, map) {
        sb.append(map.src)
        sb.append(';')
        sb.append(map.dest)
        sb.append(';')
        sb.append(map.price.getPrice('BRL'))
        sb.append(';')
        Itinerary itinerary = map.itineraries.first()
        sb.append(itinerary.routes.first().departure.format('dd/MM/yyyy HH:mm'))
        sb.append(';')
        sb.append(itinerary.routes.last().arrival.format('dd/MM/yyyy HH:mm'))
        sb.append("\n")
    }
}
