
import http.ErrorHandler;
import http.GetRequest;
import http.PostRequest;
import http.Request;
import http.RequestLogger;
import http.RequestRunner;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Arrays;

public class HttpTestTest {
    private static class ArrayRequestLogger implements RequestLogger {
        private int[][] delays;

        private int[] pos;

        private int interval;

        public ArrayRequestLogger(int interval, int iterations, int maximum) {
            delays = new int[maximum / interval][iterations];
            pos = new int[maximum / interval];
            this.interval = interval <= 0 ? 1 : interval;
        }

        public void log(Request req, long start, long end) {
            int index = (int) (req.getSize() / interval - 1);
            delays[index][pos[index]++] = (int) (end - start);
        }

        public double getDelay(long size) {
            return medium(delays[(int) (size / interval - 1)]);
        }

        private static double medium(int[] array) {
            if (array.length < 3) {
                return medium(array, 0, array.length - 1);
            } else {
                java.util.Arrays.sort(array);
                return medium(array, 1, array.length - 1 - 1);
            }
        }

        private static double medium(int[] array, int beg, int end) {
            long sum = 0;
            for (int i = beg; i <= end; i++) {
                sum += array[i];
            }

            return (double) sum / (double) (end - beg + 1);
        }
    }

    public static void main(String[] args) throws URISyntaxException {
        if (args.length < 5) {
            System.err.println("usage: java HttpTestTest <url> <method>"
                    + " <interval> <iterations> <maximum size>\n");
            return;
        }

        String method = args[1];

        /* UGH */
        if (!method.equals("GET") && !method.equals("POST")) {
            System.err.println("Invalid method: \"" + method + "\"");
            return;
        }

        String url = args[0];

        int interval = Integer.parseInt(args[2]);
        int iterations = Integer.parseInt(args[3]);
        int maximum = Integer.parseInt(args[4]);

        ErrorHandler handler = new ErrorHandler() {
            public void debug(Object o) {
                System.err.println(o);
                System.exit(-1);
            }
        };

        ArrayRequestLogger logger = new ArrayRequestLogger(interval,
                iterations, maximum);

        for (int i = 0; i < iterations; i++) {
            for (int j = interval; j <= maximum; j += interval) {
                RequestRunner runner = new RequestRunner(new URI(url), logger,
                        handler);
                if (method.equals("POST")) {
                    runner.addRequest(new PostRequest(j));
                } else {
                    runner.addRequest(new GetRequest(j));
                }
                runner.run();
				try {
					Thread.sleep(150);
				} catch (InterruptedException e) {
				}
            }
        }


        for (int i = interval; i <= maximum; i += interval) {
            System.out.println(i + "\t" + (double) logger.getDelay(i));
        }
    }
}

