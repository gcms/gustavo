package http;

import java.net.URI;
import java.net.URISyntaxException;

import java.util.Timer;
import java.util.TimerTask;

import java.io.OutputStream;
import java.io.FileOutputStream;

import base.ConnectionFactory;
import base.ErrorHandler;
import base.NoStateFactory;
import base.OutputStreamErrorHandler;
import base.OutputStreamRequestLogger;
import base.RequestLogger;
import base.RequestRunner;

public class Main {
    public static void main(String[] args) {
        if (args.length < 3) {
            System.err.println("usage: http.Main <host> <interval> <output>");
            System.exit(1);
        }

        System.setProperty("org.apache.commons.logging.Log",
                "org.apache.commons.logging.impl.NoOpLog");

        OutputStream out;

        try {
            out = new FileOutputStream(args[2]);
        } catch (java.io.IOException e) {
            System.err.println(e);
            return;
        }

        TimerTask task = new TestTimerTask(new OutputStreamRequestLogger(out),
                new OutputStreamErrorHandler(System.err), args[0]);

        Timer timer = new Timer();
        timer.scheduleAtFixedRate(task, 0, Integer.valueOf(args[1]).intValue());
    }

    private static class TestTimerTask extends TimerTask {
        private RequestLogger logger;

        private ErrorHandler handler;

        private String hostname;

        public TestTimerTask(RequestLogger logger, ErrorHandler handler,
                String hostname) {
            this.logger = logger;
            this.handler = handler;
            this.hostname = hostname;
        }

        public void run() {
            RequestRunner runner;

            try {
                ConnectionFactory connFactory = new HttpConnectionConnectionFactory(
                        new URI(hostname));
                runner = new RequestRunner(connFactory, new NoStateFactory(),
                        logger, handler);
            } catch (URISyntaxException e) {
                handler.debug(e);
                return;
            }

            for (long i = 10; i <= 10000000; i *= 10) {
                runner.addRequest(new GetRequest(i));
                runner.addRequest(new PostRequest(i));
            }

            try {
                runner.run();
            } catch (Exception e) {
                handler.debug(e);
            }
        }
    }
}