package gustavocms.airfares

import gustavocms.currency.CurrencyConverter

import java.text.DecimalFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 02/09/13
 * Time: 12:26
 * To change this template use File | Settings | File Templates.
 */
class Price implements Comparable<Price> {
    private String currency
    private BigDecimal value

    public Price(BigDecimal value, String currency) {
        this.value = value
        this.currency = currency
    }

    BigDecimal getPrice(String currency) {
        getPrice(CurrencyConverter.defaultConverter, currency)
    }

    BigDecimal getPrice(CurrencyConverter converter, String currency) {
        converter.convert(this.currency, currency, value)
    }

    String toString() {
        Currency defaultCurrency = Currency.getInstance(Locale.default)
        BigDecimal value = getPrice(CurrencyConverter.defaultConverter, defaultCurrency.getCurrencyCode())

        "${DecimalFormat.getCurrencyInstance(Locale.default).format(value)} ${defaultCurrency.getCurrencyCode()}"
    }

    public int hashCode() {
        getPrice('USD').hashCode()
    }

    public boolean equals(Price price) {
        getPrice('USD').equals(price.getPrice('USD'))
    }

    BigDecimal getAmount() {
        value
    }

    String getCurrency() {
        currency
    }

    @Override
    int compareTo(Price o) {
        getPrice('USD').compareTo(o.getPrice('USD'))
    }
}
