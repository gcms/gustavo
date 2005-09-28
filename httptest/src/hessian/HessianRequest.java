package hessian;

import java.io.IOException;

import com.caucho.hessian.client.HessianRuntimeException;

import base.Connection;
import base.ErrorHandler;
import base.Request;
import base.RequestLogger;
import base.State;

public abstract class HessianRequest implements Request {

	private long size;

	public HessianRequest(long size) {
		this.size = size;
	}

	public void request(Connection conn, State state, RequestLogger logger,
			ErrorHandler handler) throws IllegalStateException, IOException {
		if (conn == null || !conn.isOpen()) {
			throw new IllegalStateException("Invalid Connection");
		}

		if (state == null) {
			throw new IllegalStateException("Invalid State");
		}

		long start = System.currentTimeMillis();

		try {
			doRequest((HessianConnection) conn, (HessianState) state, logger,
					handler);
		} catch (HessianRuntimeException e) {
			logger.log(this, -1, -1);
			throw new IOException(e.getMessage());
		}

		long end = System.currentTimeMillis();

		logger.log(this, start, end);
	}

	public abstract void doRequest(HessianConnection conn, HessianState state,
			RequestLogger logger, ErrorHandler handler);

	public long getSize() {
		return size;
	}
}
