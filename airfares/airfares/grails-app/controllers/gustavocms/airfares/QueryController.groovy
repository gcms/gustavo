package gustavocms.airfares

import grails.converters.JSON
import grails.plugin.cache.Cacheable

class QueryController {
    QueryService queryService

    def index() {
        redirect action: 'list'
    }

    def list() {
        String query = ''
        if (params.from || params.to) {
            query += (params.from ? params.from.toUpperCase() + ';' : '')
            query += (params.to ? params.to.toUpperCase() + ';' : '')
        }

        List<Query> queries = query ? Query.findAllByContentLike("%${query}%") : Query.list()

        [ queries: queries.sort { it.currentPrice ? it.currentPrice.price : Integer.MAX_VALUE } ]
    }

    def view() {
        Query q = Query.get(params.id)
        List<BestPrice> bestPrices = queryService.getBestPricesForWeek(q)
        println bestPrices

        [ query: q, bestPrices: bestPrices, dataTable: getDataTable(q) ]
    }

    @Cacheable("bestItinerary")
    def best() {
        Query q = Query.get(params.id)

        [ itinerary: queryService.getBestItinerary(q) ]
    }

    def deals() {
        List<Deal> deals = queryService.getBestDeals()

        [ deals: deals ]
    }

    def drops() {
        List<Deal> deals = queryService.getPriceDrops()

        render view: 'deals', model: [ deals: deals ]
    }

//    def dataTable() {
//        Query q = Query.get(params.id)
//        Map result = getDataTable(q)
//
//        render (result as JSON)
//    }

    private Map getDataTable(Query q) {
        List<BestPrice> bestPrices = queryService.getBestPricesForWeek(q)

        Map result = [:]
        result.cols = [
                [id: 'day', label: 'Day', type: 'date', formatType: 'short'],
                [id: 'price', label: 'Price', type: 'number']
        ]
        result.rows = bestPrices.collect { BestPrice bp ->
            [c: [[v: "Date(${bp.day.time})"], [v: bp.price]]]
        }
        result
    }
}
