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
        List<Itinerary> itineraries = getItineraries(itinerary).unique().sort { it.getPrice() }
        if (itineraries.empty)
            return itineraries

        Price bestPrice = itineraries.first().price

        List<Itinerary> best = itineraries.findAll { it.getPrice() == bestPrice}
        best.sort { -it.getNumDays() * 24 * 60 * 60 * 1000 + it.getTotalDuration() }
    }

    @Override
    Price getBestPrice(FlightQuery itinerary) {
        getBestItineraries(itinerary).first().getPrice()
    }
}
