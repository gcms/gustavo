package gustavocms.airfares.example

import gustavocms.airfares.Price
import gustavocms.airfares.Route

import java.sql.Connection
import java.sql.DriverManager
import java.sql.PreparedStatement
import java.sql.ResultSet

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 05/09/13
 * Time: 07:42
 * To change this template use File | Settings | File Templates.
 */
class DBWriter {
    public DBWriter() {
        Class.forName("org.sqlite.JDBC")
    }

    boolean write(Date queryDate, Price price, Route leave, Route retur) {
        Integer id = get(queryDate, leave.from, retur.from, leave, retur)

        if (id != null)
            return update(id, price)

        Connection conn = DriverManager.getConnection('jdbc:sqlite:airfares.db')
        PreparedStatement stmt = conn.prepareStatement("""
INSERT INTO bestflights (queryDate, src, dst, price, currency, leaveDepart, leaveArrival, returnDepart, returnArrival)
VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
""")
        stmt.setDate(1, new java.sql.Date(queryDate.clone().clearTime().time))
        stmt.setString(2, leave.from)
        stmt.setString(3, retur.from)
        stmt.setDouble(4, price.getAmount())
        stmt.setString(5, price.getCurrency())
        stmt.setTimestamp(6, leave.departure.toTimestamp())
        stmt.setTimestamp(7, leave.arrival.toTimestamp())
        stmt.setTimestamp(8, retur.departure.toTimestamp())
        stmt.setTimestamp(9, retur.arrival.toTimestamp())
        boolean result = stmt.execute()
        stmt.close()
        conn.close()

        result
    }

    boolean update(int i, Price price) {
        Connection conn = DriverManager.getConnection('jdbc:sqlite:airfares.db')
        PreparedStatement stmt = conn.prepareStatement("""
UPDATE bestflights SET price = ?, currency = ?
WHERE id = ?
""")
        stmt.setDouble(1, price.getAmount())
        stmt.setString(2, price.getCurrency())
        stmt.setInt(3, i)

        boolean result = stmt.executeUpdate()
        stmt.close()
        conn.close()

        result
    }

    Integer get(Date queryDate, String src, String dst, Route leave, Route retur) {
        Connection conn = DriverManager.getConnection('jdbc:sqlite:airfares.db')
        PreparedStatement stmt = conn.prepareStatement("""
SELECT id FROM bestflights
where queryDate = ? and src = ? and dst = ? and leaveDepart = ? and leaveArrival = ? and returnDepart = ? and returnArrival = ?
""")
        stmt.setDate(1, new java.sql.Date(queryDate.clone().clearTime().time))
        stmt.setString(2, src)
        stmt.setString(3, dst)
        stmt.setTimestamp(4, leave.departure.toTimestamp())
        stmt.setTimestamp(5, leave.arrival.toTimestamp())
        stmt.setTimestamp(6, retur.departure.toTimestamp())
        stmt.setTimestamp(7, retur.arrival.toTimestamp())

        ResultSet rs = stmt.executeQuery()
        Integer result = rs.next() ? rs.getInt(1) : null
        rs.close()
        stmt.close()
        conn.close()

        result
    }
}
