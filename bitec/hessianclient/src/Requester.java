
import com.caucho.hessian.client.HessianProxyFactory;

public abstract class Requester {
    private int numRequests;

    private int maxSize;

    private int interval;

    public Requester(int numRequests, int maxSize, int interval) {
        this.numRequests = numRequests;
        this.maxSize = maxSize;
        this.interval = interval;
        HessianProxyFactory factory = new HessianProxyFactory();

    }

    public void request(Logger logger) {
        int[][] tmp = new int[maxSize / interval][numRequests];

        doRequest(maxSize);

        for (int i = 0; i < numRequests; i++) {
            int pos = 0;
            for (int j = interval; j <= maxSize; j += interval) {
                long start = System.currentTimeMillis();
                doRequest(j);
                long end = System.currentTimeMillis();

                tmp[pos++][i] = (int) (end - start);
                delay();
            }
        }

        for (int i = 0; i < tmp.length; i++) {
            logger.log((i + 1) * interval, medium(tmp[i]));
        }
    }

    public void delay() {
        try {
            Thread.sleep(20);
        } catch (InterruptedException e) {
        }

    }

    public abstract void doRequest(int size);

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