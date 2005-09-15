
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;

import com.caucho.hessian.client.HessianProxyFactory;

public class HessianClient implements TransferClient {

    private int numRequests;

    private int maxSize;

    private String url;

    private int interval;

    private Transfer transfer;

    public HessianClient(int numRequests, int maxSize, String url, int interval)
            throws URISyntaxException, MalformedURLException,
            ClassNotFoundException {
        this.numRequests = numRequests;
        this.maxSize = maxSize;
        this.url = url;
        this.interval = interval;
        HessianProxyFactory factory = new HessianProxyFactory();

        transfer = (Transfer) factory.create(Class.forName("Transfer"), url);

    }

    public void requestString(Logger logger) {
        new Requester(numRequests, maxSize, interval) {
            public void doRequest(int size) {
                String result = transfer.getString(size);
            }

        }.request(logger);
    }

    public void requestIntegers(Logger logger) throws ClassNotFoundException,
            MalformedURLException {
    }

    public void requestMap(Logger logger) {
        new Requester(numRequests, maxSize, interval) {
            public void doRequest(int size) {
                Map result = transfer.getMap(size);
            }

        }.request(logger);
    }

    public void sendString(Logger logger) {
        new Requester(numRequests, maxSize, interval) {
            public void doRequest(int size) {
                transfer.putString(getString(size), size);
            }
        }.request(logger);
    }

    private static StringBuffer buffer = new StringBuffer();

    private static String getString(int length) {
        if (length < 1) {
            length = 1;
        }
        if (buffer.length() < length) {
            for (int i = buffer.length(); buffer.length() < length; i++) {
                buffer.append((char) ('a' + Math.random() * ('z' - 'a')));
            }
            return buffer.toString();
        } else {
            return buffer.substring(0, length - 1);
        }
    }

    private static Map getMap(int averageSize) {
        Map map = new HashMap();
        int j = 10;
        for (int i = 0; i < averageSize; i += j + 4) {
            String str = getString(++j);
            map.put(str, new Integer(i));
        }
        return map;
    }

    public void sendMap(Logger logger) {
        new Requester(numRequests, maxSize, interval) {
            public void doRequest(int size) {
                transfer.putMap(getMap(size), size);
            }
        }.request(logger);
    }
}