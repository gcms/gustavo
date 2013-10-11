package gustavocms.airfares.despegar

import gustavocms.airfares.Itinerary
import gustavocms.airfares.SiteDirector
import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.util.JsonLoader
import org.apache.http.client.methods.HttpUriRequest

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 11/09/13
 * Time: 11:43
 * To change this template use File | Settings | File Templates.
 */
class DespegarSiteDirector implements SiteDirector {
    DespegarURLBuilder urlBuilder = new DespegarURLBuilder()
    DespegarParser parser = new DespegarParser()
    JsonLoader loader

    public DespegarSiteDirector(JsonLoader loader) {
        this.loader = loader
    }

    public List<Itinerary> loadItineraries(ItineraryQuery query) {
        if (query.oneWay) {
            HttpUriRequest url = urlBuilder.buildOneWayURL(query)
            Map map = loader.getMap(url)
            return map ? parser.parseOneWayMap(map) : []
        } else if (query.roundTrip) {
            HttpUriRequest url = urlBuilder.buildRoundTripRequest(query)
            Map map = loader.getMap(url)
            return map ? parser.parseRoundTripMap(map) : []
        } else {
            HttpUriRequest url = urlBuilder.buildMultDestURL(query)
            Map map = loader.getMap(url)
            return map ? parser.parseMultDestMap(map) : []
        }
    }
}
