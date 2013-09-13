package gustavocms.airfares

import grails.plugin.cache.Cacheable

class QueryController {
    QueryService queryService

    def index() {
        redirect action: 'list'
    }

    def list() {
        [ queries: Query.list().sort { it.currentPrice ? it.currentPrice.price : Integer.MAX_VALUE } ]
    }

    def view() {
        Query q = Query.get(params.id)
        List<BestPrice> bestPrices = queryService.getBestPricesForWeek(q)
        println bestPrices

        [ query: q, bestPrices: bestPrices ]
    }

    @Cacheable("bestItinerary")
    def best() {
        Query q = Query.get(params.id)

        [ itinerary: queryService.getBestItinerary(q) ]
    }
}
