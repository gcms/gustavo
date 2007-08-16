package base;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class BufferRequestLogger implements RequestLogger {
    public static class Log {
        public Request req;

        public long start;

        public long end;

        public Log(Request req, long start, long end) {
            this.req = req;
            this.start = start;
            this.end = end;
        }
    }

    private List logs = new ArrayList();

    private List loggers = new ArrayList();

    public void log(Request req, long start, long end) {
        logs.add(new Log(req, start, end));
        for (Iterator it = loggers.iterator(); it.hasNext(); /* */) {
            RequestLogger logger = (RequestLogger) it.next();
            logger.log(req, start, end);
        }
    }

    public void addLogger(RequestLogger logger) {
        loggers.add(logger);
    }

    public void removeLogger(RequestLogger logger) {
        loggers.remove(logger);
    }

    public void clearLoggers() {
        loggers.clear();
    }

    public void clear() {
        logs.clear();
    }

    public void logTo(RequestLogger logger) {
        for (Iterator it = logs.iterator(); it.hasNext(); /* */) {
            Log l = (Log) it.next();
            logger.log(l.req, l.start, l.end);
        }
    }

    public void loadFromFile(String filename, RequestFactory fac)
            throws IOException, IllegalArgumentException {
        loadFromFile(new File(filename), fac);
    }

    public void loadFromFile(File file, RequestFactory fac) throws IOException,
            IllegalArgumentException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(
                new FileInputStream(file)));

        String line;
        while ((line = reader.readLine()) != null) {
            String[] data = line.split("[\\s)(]+");

            String type = data[0];
            long size, start, end;

            try {
                size = Long.valueOf(data[1]).longValue();
                start = Long.valueOf(data[2]).longValue();
                end = Long.valueOf(data[3]).longValue();
            } catch (NumberFormatException e) {
                throw new IllegalArgumentException("Invalid file");
            }

            Request req = fac.getRequest(type, size);

            if (req == null) {
                throw new IllegalArgumentException("Invalid file");
            }

            logs.add(new Log(req, start, end));
        }
    }
}
