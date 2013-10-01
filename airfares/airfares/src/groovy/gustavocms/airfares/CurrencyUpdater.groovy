package gustavocms.airfares

import gustavocms.currency.CurrencyConverter

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 13/09/13
 * Time: 08:24
 * To change this template use File | Settings | File Templates.
 */
class CurrencyUpdater  implements Runnable {
    private Thread thread
    private boolean ativo = false

    public void run() {
        while (ativo) {
            Date now = new Date()
            int hora = now.toCalendar().get(Calendar.HOUR_OF_DAY)
            if (0 <= hora && hora <= 2) {
                if (!ExchangeRate.findByCurrencyAndDay('BRL', now.clearTime())) {
                    BigDecimal rate = CurrencyConverter.defaultConverter.convert('USD', 'BRL', 1.0)
                    new ExchangeRate(currency: 'BRL', day: now, rate: rate).save()
                }
            }

            Thread.sleep(1000 * 60 * 30)
        }
    }

    void start() {
        if (thread != null)
            throw new IllegalStateException()

        thread = new Thread(this)
        ativo = true
        thread.start()
    }

    void stop() {
        if (!ativo)
            throw new IllegalStateException()

        ativo = false
    }
}
