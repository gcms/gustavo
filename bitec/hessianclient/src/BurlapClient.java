import java.net.MalformedURLException;
import java.net.URISyntaxException;

import com.caucho.burlap.client.BurlapProxyFactory;

public class BurlapClient implements TransferClient {

	private int numRequests;

	private int maxSize;

	private String url;

	public BurlapClient(int numRequests, int maxSize, String url)
			throws URISyntaxException {
		this.numRequests = numRequests;
		this.maxSize = maxSize;
		this.url = url;
	}

	public void requestString(Logger logger) throws ClassNotFoundException,
			MalformedURLException {
		BurlapProxyFactory factory = new BurlapProxyFactory();

		Transfer transfer = (Transfer) factory.create(
				Class.forName("Transfer"), url);

		double[] delays = new double[maxSize];
		long[] tmp = new long[maxSize];

		transfer.getString(maxSize);

		for (int j = 0; j < numRequests; j++) {
			for (int i = 0; i < maxSize; i++) {
				long start = System.currentTimeMillis();
				transfer.getString(i + 1);
				long end = System.currentTimeMillis();
				tmp[i] += end - start;

				if (j == numRequests - 1) {
					delays[i] = (double) tmp[i] / (double) numRequests;
					logger.log(i, delays[i]);
				}
			}
		}
	}

	public void requestIntegers(Logger logger) throws ClassNotFoundException,
			MalformedURLException {
		BurlapProxyFactory factory = new BurlapProxyFactory();

		Transfer transfer = (Transfer) factory.create(
				Class.forName("Transfer"), url);

		double[] delays = new double[maxSize];
		long[] tmp = new long[maxSize];

		transfer.getIntegers(maxSize);

		for (int j = 0; j < numRequests; j++) {
			for (int i = 0; i < maxSize; i++) {
				long start = System.currentTimeMillis();
				transfer.getIntegers(i + 1);
				long end = System.currentTimeMillis();
				tmp[i] += end - start;

				if (j == numRequests - 1) {
					delays[i] = (double) tmp[i] / (double) numRequests;
					logger.log(i, delays[i]);
				}
			}
		}
	}

	public void requestMap(Logger logger) throws ClassNotFoundException,
			MalformedURLException {
		BurlapProxyFactory factory = new BurlapProxyFactory();

		Transfer transfer = (Transfer) factory.create(
				Class.forName("Transfer"), url);

		double[] delays = new double[maxSize];
		long[] tmp = new long[maxSize];

		transfer.getMap(maxSize);

		for (int j = 0; j < numRequests; j++) {
			for (int i = 0; i < maxSize; i++) {
				long start = System.currentTimeMillis();
				transfer.getMap(i + 1);
				long end = System.currentTimeMillis();
				tmp[i] += end - start;

				if (j == numRequests - 1) {
					delays[i] = (double) tmp[i] / (double) numRequests;
					logger.log(i, delays[i]);
				}
			}
		}
	}
}
