package gustavocms.airfares

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 13/09/13
 * Time: 08:23
 * To change this template use File | Settings | File Templates.
 */
class ExchangeRate {
    Date day
    String currency
    BigDecimal rate

    static mapping = {
        day sqlType: 'date', unique: 'currency', scale: 4
    }
}
