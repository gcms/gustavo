package gustavocms.currency

import gustavocms.airfares.cache.Cache
import gustavocms.airfares.cache.MemoryCache

import java.math.RoundingMode
import java.text.DecimalFormat
import java.text.NumberFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 05/09/13
 * Time: 07:52
 * To change this template use File | Settings | File Templates.
 */
class YahooCurrencyConverter implements CurrencyConverter {
    private NumberFormat format = DecimalFormat.getInstance(new Locale('en', 'US'))
    private Cache cache = new MemoryCache()

    @Override
    BigDecimal convert(Currency current, Currency target, BigDecimal value) {
        if (current == target)
            return value.setScale(2, RoundingMode.HALF_DOWN)

        String quote = current.currencyCode + target.currencyCode

        Date today = new Date()
        String result = cache.get(today.format('yyyyMMdd') + quote) ?: fetchRate(today, quote)
        Number rate = format.parse(result)

        (value * rate.toBigDecimal()).setScale(2, RoundingMode.HALF_DOWN)
    }

    String fetchRate(Date today, String quote) {
        String result = new URL("http://download.finance.yahoo.com/d/quotes.csv?s=${quote}=X&f=l1&e=.csv").newReader().text.replaceAll('\\s+', '')
        cache.put(today.format('yyyyMMdd') + quote, result)
        result
    }

    @Override
    BigDecimal convert(String current, String target, BigDecimal value) {
        convert(Currency.getInstance(current), Currency.getInstance(target), value)
    }
}
