package gustavocms.currency

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 05/09/13
 * Time: 07:52
 * To change this template use File | Settings | File Templates.
 */
public interface CurrencyConverter {
    static CurrencyConverter defaultConverter = new YahooCurrencyConverter()

    BigDecimal convert(Currency current, Currency target, BigDecimal value)
    BigDecimal convert(String current, String target, BigDecimal value)
}