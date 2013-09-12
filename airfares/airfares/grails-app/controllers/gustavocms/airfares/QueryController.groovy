package gustavocms.airfares

class QueryController {
    QueryService queryService

    def index() {
        redirect action: 'list'
    }

    def list() {
        [ queries: Query.list() ]
    }

    def view() {
        Query q = Query.get(params.id)
        List<BestPrice> bestPrices = queryService.getBestPricesForWeek(q)
        println bestPrices

        [ query: q, bestPrices: bestPrices ]
    }
}
