package gustavocms.airfares.decolar

import groovy.json.JsonSlurper
import gustavocms.airfares.Itinerary
import gustavocms.airfares.Price
import gustavocms.airfares.Provider
import gustavocms.airfares.cache.Cache
import gustavocms.airfares.query.DateTimeInterval
import gustavocms.airfares.query.FlightQuery

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 12:27
 * To change this template use File | Settings | File Templates.
 */
class DecolarProviderTests extends GroovyTestCase {
    void setUp() {
        String today = new Date().format('yyyyMMdd')
        new File('testdb').eachFile { File file ->
            Cache.DEFAULT.put('decolar' + today + file.name, new JsonSlurper().parseText(file.text).conteudo)
        }
    }

    void tearDown() {
        String today = new Date().format('yyyyMMdd')
        new File('testdb').eachFile { File file ->
            Cache.DEFAULT.remove(today + file.name)
        }
    }

    void testParse() {
//        String key = DecolarLoader.getKey(new URL("http://www.us.despegar.com/shop/flights/data/search/roundtrip/gyn/dub/2014-02-07/2014-03-05/1/0/0/TOTALFARE/ASCENDING/NA/NA/NA/NA/NA"))
//
//        Cache.DEFAULT.remove(key)
//        Cache.DEFAULT.put(key, getClass().getResourceAsStream('/roundTripGYNDUB.json').newReader().text)

        Provider provider = new DecolarProvider()

        FlightQuery itinerary = FlightQuery.createRoundTripQuery(
                'GYN', 'MIA',
                DateTimeInterval.getIntervalFullDay(Date.parse('dd/MM/yyyy', '07/02/2014')),
                DateTimeInterval.getIntervalFullDay(Date.parse('dd/MM/yyyy', '05/03/2014')))

        List<Itinerary> bestItineraries = provider.getBestItineraries(itinerary)
        Price price = bestItineraries.first().getPrice()
        assertEquals 1131, price.getPrice('USD')
    }

    void testDublin() {
//        String key = DecolarLoader.getKey(new URL("http://www.us.despegar.com/shop/flights/data/search/roundtrip/gyn/dub/2014-02-07/2014-03-05/1/0/0/TOTALFARE/ASCENDING/NA/NA/NA/NA/NA"))
//
//        Cache.DEFAULT.remove(key)
//        Cache.DEFAULT.put(key, getClass().getResourceAsStream('/roundTripGYNDUB.json').newReader().text)

        Provider provider = new DecolarProvider()

        FlightQuery itinerary = FlightQuery.createRoundTripQuery(
                'GYN', 'DUB',
                DateTimeInterval.getIntervalFullDay(Date.parse('dd/MM/yyyy', '07/02/2014')),
                DateTimeInterval.getIntervalFullDay(Date.parse('dd/MM/yyyy', '05/03/2014')))

        Price price = provider.getBestPrice(itinerary)

        assertEquals 1327, price.getPrice('USD')

    }

    void testOneWay() {
//        String key = DecolarLoader.getKey(new URL("http://www.us.despegar.com/shop/flights/data/search/oneway/bsb/mia/2014-02-08/1/0/0/TOTALFARE/ASCENDING/NA/NA/NA/NA"))
//
//        Cache.DEFAULT.remove(key)
//        Cache.DEFAULT.put(key, getClass().getResourceAsStream('/oneWayBSBMIA.json').newReader().text)

        Provider provider = new DecolarProvider()

        FlightQuery fq = FlightQuery.createOneWay('BSB', 'MIA', Date.parse('dd/MM/yyyy', '08/02/2014'))

        Price price = provider.getBestPrice(fq)
        assertEquals 1127, price.getPrice('USD')
    }

    void testWebService() {
        Provider provider = new DecolarProvider()

        FlightQuery q = FlightQuery.createRoundTripQuery(
                'BSB', 'MIA',
                DateTimeInterval.getIntervalFullDay(Date.parse('dd/MM/yyyy', '07/02/2014')),
                DateTimeInterval.getIntervalFullDay(Date.parse('dd/MM/yyyy', '05/03/2014')))

        List itineraries = provider.getItineraries(q)
        assertFalse itineraries.empty
        itineraries.each { Itinerary itinerary ->
            assertEquals 2, itinerary.routes.size()
            assertNotNull itinerary.price
            assertNotNull itinerary.price.getPrice('USD')
        }
    }
}
