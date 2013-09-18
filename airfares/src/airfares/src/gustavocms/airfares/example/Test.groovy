package gustavocms.airfares.example

import java.sql.Connection
import java.sql.DriverManager
import java.sql.Statement

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 04/09/13
 * Time: 15:21
 * To change this template use File | Settings | File Templates.
 */
class Test {
    static void main(String[] args) {
        Class.forName("org.sqlite.JDBC")

        new File('airfares.db').delete()
        Connection conn = DriverManager.getConnection('jdbc:sqlite:airfares.db')
        Statement stmt = conn.createStatement()
        stmt.setQueryTimeout(30)

        stmt.executeUpdate("""
CREATE TABLE bestflights (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  queryDate date,
  src char(5),
  dst char(5),
  price decimal,
  currency char(5),
  leaveDepart timestamp,
  leaveArrival timestamp,
  returnDepart timestamp,
  returnArrival timestamp
)
""")

    }
}
