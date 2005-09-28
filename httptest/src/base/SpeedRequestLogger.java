package base;

import java.text.DecimalFormat;
import java.util.Observable;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

public class SpeedRequestLogger extends Observable implements RequestLogger {
    private List logs = new ArrayList();

    public void log(Request req, long start, long end) {
        log(new BufferRequestLogger.Log(req, start, end));
    }

    public void log(BufferRequestLogger.Log log) {
        logs.add(log);
        setChanged();
        notifyObservers();
    }

    public double getSpeed() {
        double s = 0;
        // long data = 0;
        // double delay = 0;

        for (Iterator it = logs.iterator(); it.hasNext(); /* */) {
            BufferRequestLogger.Log log = (BufferRequestLogger.Log) it.next();
            // data += log.req.getSize();
            // delay += log.end - log.start;

            if (log.start > 0 && log.end > 0) {
                s += (double) log.req.getSize()
                        / (double) (log.end - log.start);
            }
        }

        clearChanged();

        // return (double) data / delay;
        return s / (double) logs.size();
    }

    public String toString() {
        return new DecimalFormat("#.##").format(getSpeed()) + " KB/s";
    }
}