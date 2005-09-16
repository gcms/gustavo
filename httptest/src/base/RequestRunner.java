package base;

import java.io.IOException;

import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

public class RequestRunner extends Runner {
	static {
		System.setProperty("org.apache.commons.logging.Log",
				"org.apache.commons.logging.impl.NoOpLog");
	}

	private Connection conn;

	private ConnectionFactory connFactory;

	private StateFactory stateFactory;

	private RequestLogger logger;

	private ErrorHandler handler;

	private boolean wantStop;

	protected List requests;

	public RequestRunner(List requests, ConnectionFactory connFactory,
			StateFactory stateFactory, RequestLogger logger,
			ErrorHandler handler) {
		this.requests = requests;
		this.stateFactory = stateFactory;
		this.connFactory = connFactory;
		this.logger = logger;
		this.handler = handler;
	}

	public RequestRunner(ConnectionFactory connFactory,
			StateFactory stateFactory, RequestLogger logger,
			ErrorHandler handler) {
		this(new ArrayList(), connFactory, stateFactory, logger, handler);
	}

	public RequestRunner(ConnectionFactory connFactory) {
		this(connFactory, new NoStateFactory(), new OutputStreamRequestLogger(
				System.out), new OutputStreamErrorHandler(System.err));
	}

	public RequestRunner(RequestRunner runner) {
		this(runner.requests, runner.connFactory, runner.stateFactory,
				runner.logger, runner.handler);
	}

	public RequestRunner() {
		this(null, null, null, null, null);
	}

	public void setConnectionFactory(ConnectionFactory connFactory) {
		this.connFactory = connFactory;
	}

	public ConnectionFactory getConnectionFactory() {
		return connFactory;
	}

	public void setStateFactory(StateFactory stateFactory) {
		this.stateFactory = stateFactory;
	}

	public StateFactory getStateFactory() {
		return stateFactory;
	}

	public void setRequestLogger(RequestLogger logger) {
		this.logger = logger;
	}

	public RequestLogger getRequestLogger() {
		return logger;
	}

	public void setErrorHandler(ErrorHandler handler) {
		this.handler = handler;
	}

	public ErrorHandler getErrorHandler() {
		return handler;
	}

	public Object clone() {
		RequestRunner runner = new RequestRunner(connFactory, stateFactory,
				logger, handler);
		for (Iterator it = requests.iterator(); it.hasNext(); /* */) {
			runner.requests.add(it.next());
		}

		return runner;
	}

	/*
	 * public RequestRunner(RequestRunner runner) { this.requests = (List)
	 * runner.requests.clone(); this.uri = runner.uri; this.logger =
	 * runner.logger; this.handler = runner.handler; }
	 */

	public void addRequest(Request req) {
		requests.add(req);
	}

	public void addRequest(int index, Request req) {
		requests.add(index, req);
	}

	public Object removeRequest(int index) {
		return requests.remove(index);
	}

	public boolean removeRequest(Request req) {
		return requests.remove(req);
	}

	public Object getRequest(int index) {
		return requests.get(index);
	}

	public int numRequests() {
		return requests.size();
	}

	/**
	 * Asks for the run to stop, a flag will be set to this run stating that it
	 * wants to stop. If it's called before the run() method starts, or after it
	 * ends, it will have no effect. It can be useful if you're dealing with
	 * threads though.
	 */
	public void stop() {
		if (!wantStop) {
			wantStop = true;
			if (conn != null && conn.isOpen()) {
				new Thread(new Runnable() {
					public void run() {
						conn.close();
					}
				}).start();
			}
		}
	}

	public void run() {
		try {
			conn = connFactory.getConnection();
		} catch (IllegalArgumentException e) {
			handler.debug(e.getMessage());
			notifyListeners(new NotifyEvent(this));
			return;
		}

		State state = stateFactory.getState();

		if (wantStop) {
			notifyListeners(new NotifyEvent(this));
			return;
		}

		try {
			conn.open();
		} catch (IOException e) {
			handler.debug(e.getMessage());
			notifyListeners(new NotifyEvent(this));
			return;
		}

		for (Iterator it = requests.iterator(); it.hasNext(); /* */) {
			Request req = (Request) it.next();

			try {
				req.request(conn, state, logger, handler);
			} catch (IOException e) {
				if (wantStop) {
					/* if we want it to stop, we just break the loop */
					break;
				} else {
					handler.debug(e.getMessage());
					continue;
				}
			} catch (IllegalStateException e) {
				if (!wantStop) {
					handler.debug(e.getMessage());
				}
				break;
			} catch (IllegalArgumentException e) {
				if (!wantStop) {
					handler.debug(e.getMessage());
				}
				break;
			}
		}

		conn.close();

		/* notify listeners that this runner finished running */
		notifyListeners(new NotifyEvent(this));
	}
}