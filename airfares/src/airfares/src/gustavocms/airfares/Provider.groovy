package gustavocms.airfares

import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.fetch.ItineraryQueryType

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 12:24
 * To change this template use File | Settings | File Templates.
 */
public interface Provider {
    public List<Itinerary> getItineraries(FlightQuery itinerary)
    public List<Itinerary> getBestItineraries(FlightQuery itinerary)
    public Price getBestPrice(FlightQuery itinerary)
    public Collection<ItineraryQueryType> getSupportedQueries()
}