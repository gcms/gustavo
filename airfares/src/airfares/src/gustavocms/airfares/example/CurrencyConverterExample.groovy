package gustavocms.airfares.example

import gustavocms.currency.YahooCurrencyConverter

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 05/09/13
 * Time: 07:49
 * To change this template use File | Settings | File Templates.
 */
class CurrencyConverterExample {
    static void main(String[] args) {
        println new YahooCurrencyConverter().convert('USD', 'BRL', 10)
    }
}
