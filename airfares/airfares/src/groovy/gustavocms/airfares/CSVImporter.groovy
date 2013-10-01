package gustavocms.airfares

import liquibase.util.csv.opencsv.CSVReader

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 13/09/13
 * Time: 11:22
 * To change this template use File | Settings | File Templates.
 */
class CSVImporter {
    static void importFile() {
        DateFormat df = new SimpleDateFormat('yyyy-MM-dd')

        CSVReader csv = new CSVReader(new File('D:/prices.csv').newReader())
        csv.readNext()
        csv.readAll().each { values ->
            values = values.collect { String val -> val.stripMargin('"') }
            println values
            Date day = df.parse(values[0]).clearTime()
            String src = values[1]
            String dst = values[2]
            println values[3]
            BigDecimal price = new BigDecimal(values[3])
            String currency = values[4]

            Query q = Query.findByContentLike("${src};${dst};%")
            if (q != null && !BestPrice.findByQueryAndDay(q, day)) {
                new BestPrice(day: day, query: q, price: price, currency: currency).save()
            }
        }
    }
}