import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.RemoteException;

import org.apache.axis.client.Call;
import org.apache.axis.client.Service;
import org.apache.axis.encoding.XMLType;
import javax.xml.rpc.ParameterMode;
import javax.xml.rpc.ServiceException;

public class AxisClient implements TransferClient {

	private int numRequests;

	private int maxSize;

	private URL url;

	public AxisClient(int numRequests, int maxSize, String url)
			throws MalformedURLException {
		this(numRequests, maxSize, new URL(url));
	}

	public AxisClient(int numRequests, int maxSize, URL url) {
		this.numRequests = numRequests;
		this.maxSize = maxSize;
		this.url = url;
	}

	public void requestString(Logger logger) throws ServiceException,
			RemoteException {
		double[] delays = new double[maxSize];
		long[] tmp = new long[maxSize];

		String method = "getString";

		Service service = new Service();
		Call call = (Call) service.createCall();

		call.setTargetEndpointAddress(url);
		call.setOperationName(method);
		call.addParameter("size", XMLType.XSD_INT, ParameterMode.IN);
		call.setReturnType(XMLType.XSD_STRING);

		for (int j = 0; j < numRequests; j++) {
			for (int i = 0; i < maxSize; i++) {
				long start = System.currentTimeMillis();

				call.invoke(new Object[] { new Integer(i + 1) });

				long end = System.currentTimeMillis();
				tmp[i] += end - start;

				if (j == numRequests - 1) {
					delays[i] = (double) tmp[i] / (double) numRequests;
					logger.log(i, delays[i]);
				}
			}
		}
	}

	public void requestIntegers(Logger logger) throws ServiceException,
			RemoteException {
		double[] delays = new double[maxSize];
		long[] tmp = new long[maxSize];

		String method = "getIntegers";

		Service service = new Service();
		Call call = (Call) service.createCall();

		call.setTargetEndpointAddress(url);
		call.setOperationName(method);
		call.addParameter("size", XMLType.XSD_INT, ParameterMode.IN);
		call.setReturnType(XMLType.SOAP_ARRAY);

		for (int j = 0; j < numRequests; j++) {
			for (int i = 0; i < maxSize; i++) {
				long start = System.currentTimeMillis();

				call.invoke(new Object[] { new Integer(i + 1) });

				long end = System.currentTimeMillis();
				tmp[i] += end - start;

				if (j == numRequests - 1) {
					delays[i] = (double) tmp[i] / (double) numRequests;
					logger.log(i, delays[i]);
				}
			}
		}
	}

	public void requestMap(Logger logger) throws ServiceException,
			RemoteException {
		double[] delays = new double[maxSize];
		long[] tmp = new long[maxSize];

		String method = "getMap";

		Service service = new Service();
		Call call = (Call) service.createCall();

		call.setTargetEndpointAddress(url);
		call.setOperationName(method);
		call.addParameter("size", XMLType.XSD_INT, ParameterMode.IN);
		call.setReturnType(XMLType.SOAP_MAP);

		for (int j = 0; j < numRequests; j++) {
			for (int i = 0; i < maxSize; i++) {
				long start = System.currentTimeMillis();

				call.invoke(new Object[] { new Integer(i + 1) });

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
