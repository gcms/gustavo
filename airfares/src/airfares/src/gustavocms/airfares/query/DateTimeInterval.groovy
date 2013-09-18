package gustavocms.airfares.query

import org.apache.commons.lang3.time.DateUtils

import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created with IntelliJ IDEA.
 * User: gustavo
 * Date: 10/09/13
 * Time: 16:29
 * To change this template use File | Settings | File Templates.
 */
class DateTimeInterval {
    private Date start
    private Date end

    public DateTimeInterval(Date start, Date end) {
        this.start = start
        this.end = end
    }

    public DateTimeInterval(Date start, int nDays) {
        this.start = start
        this.end = start + nDays
    }

    public Date getStart() {
        this.start
    }

    public Date getEnd() {
        this.end
    }

    public static DateTimeInterval getIntervalFullDay(Date day) {
        new DateTimeInterval(day.clone().clearTime(), DateUtils.addMilliseconds(day.clone().clearTime() + 1, -1))
    }

    public void eachDay(int startStep = 0, Closure cl) {
        for (Date day = start.clone().clearTime() + startStep; day <= end; day++) {
            cl(day)
        }
    }

    public Collection<Date> getDays(int startStep = 0) {
        List<Date> days = []
        eachDay(startStep) { Date day ->
            days << day
        }
        days
    }

    boolean contains(Date date) {
        start <= date && date <= end
    }

    String toString() {
        DateFormat format = new SimpleDateFormat('dd/MM/yyyy HH:mm')
        "${format.format(start)} - ${format.format(end)}"
    }
}
