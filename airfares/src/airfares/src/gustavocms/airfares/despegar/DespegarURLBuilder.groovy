package gustavocms.airfares.despegar

import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.query.fetch.ItineraryQueryRoute
import org.apache.http.HttpRequest
import org.apache.http.client.methods.HttpGet
import org.apache.http.client.methods.HttpUriRequest

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 12:38
 * To change this template use File | Settings | File Templates.
 */
class DespegarURLBuilder {
    public HttpUriRequest buildRoundTripRequest(ItineraryQuery query) {
        DateFormat format = new SimpleDateFormat('yyyy-MM-dd')

        ItineraryQueryRoute leave = query.routes.first()
        ItineraryQueryRoute retur = query.routes.last()

        String from = leave.from.toLowerCase()
        String to = leave.to.toLowerCase()

        String leaveDay = format.format(leave.day)
        String returDay = format.format(retur.day)

        String url = "http://www.us.despegar.com/shop/flights/data/search/roundtrip/${from}/${to}/${leaveDay}/${returDay}/1/0/0/TOTALFARE/ASCENDING/NA/NA/NA/NA/NA"
        String referer =  "http://www.us.despegar.com/shop/flights/results/roundtrip/${from.toUpperCase()}/${to.toUpperCase()}/${leaveDay}/${returDay}/1/0/0"

        HttpGet get = new HttpGet(url.toURI())
        setHeaders(get, referer)
        get
    }

    private static void setHeaders(HttpRequest get, String referer) {
        get.setHeader('Accept', 'application/json, text/javascript, */*; q=0.01')
        get.setHeader('Accept-Encoding', 'gzip,deflate,sdch')
        get.setHeader('Accept-Language', 'en-US,en;q=0.8,pt-BR;q=0.6,pt;q=0.4,fr;q=0.2,es;q=0.2')
        get.setHeader('Host', 'www.us.despegar.com')
        get.setHeader('Proxy-Connection', 'keep-alive')
        get.setHeader('Referer', referer)
        get.setHeader('X-Requested-With', 'XMLHttpRequest')
    }

    public HttpUriRequest buildMultDestURL(ItineraryQuery query) {
        DateFormat format = new SimpleDateFormat('yyyy-MM-dd')

        String from = query.routes.collect { it.from.toLowerCase() }.join(',')
        String to = query.routes.collect { it.to.toLowerCase() }.join(',')
        String departures = query.routes.collect { format.format(it.day) }.join(',')
//                      http://www.decolar.com/shop/flights/data/search/multipledestinations/gyn,par,dub/par,dub,gyn/2014-02-07,2014-02-18,2014-03-04/1/0/0/FARE/ASCENDING
        String url = "http://www.us.despegar.com/shop/flights/data/search/multipledestinations/${from}/${to}/${departures}/1/0/0/TOTALFARE/ASCENDING"
        String referer = "http://www.us.despegar.com/shop/flights/results/multipledestinations/${from.toUpperCase()}/${to.toUpperCase()}/${departures}/1/0/0"

        HttpGet get = new HttpGet(url.toURI())
        setHeaders(get, referer)
        get
//        http://www.us.despegar.com/shop/flights/data/search/multipledestinations/gyn,mvd,sao/bue,sao,gyn/2013-11-19,2013-12-01,2013-12-11/1/0/0/TOTALFARE/ASCENDING
//
//        http://www.us.despegar.com/shop/flights/data/search/multipledestinations/gyn,mvd/bue,gyn/2013-11-19,2013-12-01/1/0/0/TOTALFARE/ASCENDING
    }

    public HttpUriRequest buildOneWayURL(ItineraryQuery query) {
        DateFormat format = new SimpleDateFormat('yyyy-MM-dd')

        ItineraryQueryRoute route = query.routes.first()
        String from = route.from.toLowerCase()
        String to = route.to.toLowerCase()
        String day = format.format(route.day)

        String url = "http://www.us.despegar.com/shop/flights/data/search/oneway/${from}/${to}/${day}/1/0/0/TOTALFARE/ASCENDING/NA/NA/NA/NA"
        String referer = "http://www.us.despegar.com/shop/flights/results/oneway/${from.toUpperCase()}/${to.toUpperCase()}/${day}/1/0/0"

        HttpGet get = new HttpGet(url.toURI())
        setHeaders(get, referer)
        get
    }
}
