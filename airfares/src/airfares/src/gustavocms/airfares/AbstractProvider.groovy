package gustavocms.airfares

import gustavocms.airfares.query.FlightQuery

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 10/09/13
 * Time: 10:23
 * To change this template use File | Settings | File Templates.
 */
abstract class AbstractProvider implements Provider {

    @Override
    List<Itinerary> getBestItineraries(FlightQuery itinerary) {
        List<Itinerary> itineraries = getItineraries(itinerary)
        itineraries.empty ? itineraries : getBestItinerariesSorted(itineraries)
    }

    protected List<Itinerary> getBestItinerariesSorted(List<Itinerary> itineraries) {
        List<Itinerary> sorted = itineraries.unique().sort { it.price }

        Price bestPrice = sorted.first().price
        List<Itinerary> best = sorted.findAll { it.price == bestPrice }
        best.sort { -it.numDays * 24 * 60 * 60 * 1000 + it.totalDuration }
    }

    @Override
    Price getBestPrice(FlightQuery itinerary) {
        List<Itinerary> itineraries = getBestItineraries(itinerary)
        itineraries.empty ? null : itineraries.first().getPrice()
    }
}
