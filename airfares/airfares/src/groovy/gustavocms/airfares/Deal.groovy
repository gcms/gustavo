package gustavocms.airfares

import java.math.RoundingMode

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 16/09/13
 * Time: 09:04
 * To change this template use File | Settings | File Templates.
 */
class Deal {
    Query query
    BigDecimal average

    BigDecimal getAverage() {
        average.setScale(2, RoundingMode.HALF_DOWN)
    }

    BigDecimal getDiscount() {
        (1 - query.currentPrice.price / average).multiply(100).setScale(2, RoundingMode.HALF_DOWN)
    }
}
