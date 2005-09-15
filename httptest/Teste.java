import http.RequestLogger;
import http.ErrorHandler;

import http.Request;
import http.RequestRunner;
import http.PostRequest;
import http.PostRequest;

import java.util.Map;
import java.util.HashMap;

import java.net.URI;
import java.net.URISyntaxException;

public class Teste {
    private static class MapRequestLogger implements RequestLogger {
        private Map map = new HashMap();

        public void log(Request req, long start, long end) {
            Long val = (Long) map.get(new Long(req.getSize()));
            val = val == null ? new Long(end - start)
                : new Long(val.longValue() + end - start);
            map.put(new Long(req.getSize()), val);
        }

        public long getDelay(long size) {
            Long del = (Long) map.get(new Long(size));
            return del == null ? -1 : del.longValue();
        }
    }

    public static void main(String[] args) throws URISyntaxException {
        if (args.length < 4) {
            System.err.println("usage: java Teste <url> <interval>"
               + " <iterations> <maximum size>\n");
            return;
        }

        String url = args[0];

        MapRequestLogger logger = new MapRequestLogger();

        ErrorHandler handler = new ErrorHandler() {
            public void debug(Object o) {
                System.err.println(o);
                System.exit(-1);
            }
        };


        int interval = Integer.parseInt(args[1]);
        int iterations = Integer.parseInt(args[2]);
        int maximum = Integer.parseInt(args[3]);

        for (int i = 0; i < iterations; i++) {
            for (int j = interval; j <= maximum; j += interval) {
                RequestRunner runner = new RequestRunner(new URI(url),
                        logger, handler);
                runner.addRequest(new PostRequest(j));
                runner.run();
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                }
            }
        }

        for (int i = interval; i <= maximum; i += interval) {
            System.out.println(i + "\t"
                    + (double) logger.getDelay(i) / (double) iterations);
        }
    }
}   

