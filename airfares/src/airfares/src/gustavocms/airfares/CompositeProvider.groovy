package gustavocms.airfares

import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.fetch.ItineraryQueryType

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 17/09/13
 * Time: 09:58
 * To change this template use File | Settings | File Templates.
 */
class CompositeProvider extends AbstractProvider implements Provider {
    private Set<Provider> providers = new HashSet<Provider>()

    public CompositeProvider(Collection<Provider> providers) {
        this.providers.addAll(providers)
    }

    public CompositeProvider(Provider main) {
        providers << main
    }

    @Override
    List<Itinerary> getItineraries(FlightQuery query) {
        providers.collect { it.getItineraries(query) }.flatten()
    }

    @Override
    Collection<ItineraryQueryType> getSupportedQueries() {
        providers.collect { it.supportedQueries }.flatten().unique()
    }
}
