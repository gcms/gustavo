package gustavocms.airfares.travelocity

import gustavocms.airfares.AbstractProvider
import gustavocms.airfares.Itinerary
import gustavocms.airfares.SiteDirector
import gustavocms.airfares.decolar.DecolarLoader
import gustavocms.airfares.LoadThread
import gustavocms.airfares.query.FlightQuery
import gustavocms.airfares.query.fetch.ItineraryQuery
import gustavocms.airfares.query.fetch.ItineraryQueryType
import org.apache.commons.logging.Log
import org.apache.commons.logging.LogFactory

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 12:26
 * To change this template use File | Settings | File Templates.
 */
class TravelocityProvider extends AbstractProvider {
    private static Log log = LogFactory.getLog(TravelocityProvider)
    private SiteDirector director = new TravelocitySiteDirector(new TravelocityLoader())

    @Override
    List<Itinerary> getItineraries(FlightQuery fq) {
        List<LoadThread> threads = new ArrayList<LoadThread>()

        fq.getQueries().each { ItineraryQuery query ->
            def thread = new LoadThread(director: director, query: query)
            threads << thread
            thread.start()
        }

        List<Itinerary> result = []
        for (int i = 0; i < threads.size(); i++) {
            Thread th = threads[i]
            th.join()
            result.addAll(th.result.findAll { fq.filter(it) })
        }

        println ' finish'

        result
    }

    @Override
    Collection<ItineraryQueryType> getSupportedQueries() {
        ItineraryQueryType.values()
    }
}
