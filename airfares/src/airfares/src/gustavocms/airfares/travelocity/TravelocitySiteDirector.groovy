package gustavocms.airfares.travelocity

import gustavocms.airfares.Itinerary
import gustavocms.airfares.SiteDirector
import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.util.JsonLoader

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 16/09/13
 * Time: 12:53
 * To change this template use File | Settings | File Templates.
 */
class TravelocitySiteDirector implements SiteDirector {
    JsonLoader loader
    TravelocityParser parser = new TravelocityParser()
    TravelocityURLBuilder urlBuilder = new TravelocityURLBuilder()

    public TravelocitySiteDirector(JsonLoader loader) {
        this.loader = loader
    }

    public List<Itinerary> loadItineraries(ItineraryQuery query) {
        if (query.roundTrip) {
            URL url = urlBuilder.buildRoundTripURL(query)
            Map map = loader.getMap(url)
            return map ? loadItineraries(query, map) : []
        }

        throw new UnsupportedOperationException()
    }

    public List<Itinerary> loadItineraries(ItineraryQuery query, Map json) {
        String rid = json.metadata.responseId
        json.results.collect { Map result ->
            loadItineraries(query, rid, result)
        }.flatten()
    }

    public List<Itinerary>  loadItineraries(ItineraryQuery query, String rid, Map outbound) {
        URL url = urlBuilder.buildInboundURLQuery(query, rid, outbound.uniqueId as String)
        Map json = loader.getMap(url)

        json ? collectResults(json, outbound) : []
    }

    private List<Itinerary> collectResults(Map json, Map outbound) {
        json?.results.collect { Map inbound ->
            parser.parseItinerary(outbound, inbound)
        }
    }

}
