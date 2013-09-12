package gustavocms.airfares

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 12/09/13
 * Time: 11:27
 * To change this template use File | Settings | File Templates.
 */
class BestPrice {
    Query query
    static belongsTo = [query: Query]

    Date day
    BigDecimal price
    String currency

    static mapping = {
        day sqlType: 'date'//, unique: 'query'
    }
}
