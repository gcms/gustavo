package gustavocms.airfares

import gustavocms.airfares.query.fetch.ItineraryQuery

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 17/09/13
 * Time: 08:53
 * To change this template use File | Settings | File Templates.
 */
public interface SiteDirector {
    public List<Itinerary> loadItineraries(ItineraryQuery query)
}