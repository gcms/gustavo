package http;

import base.Request;
import base.RequestFactory;

public class HttpRequestFactory implements RequestFactory {

	public Request getRequest(String param) {
		return null;
	}

	public Request getRequest() {
		return null;
	}

	public Request getRequest(String param, long size) {
		return param.equals("GET") ? (Request) new GetRequest(size) : param
				.equals("POST") ? (Request) new PostRequest(size) : null;
	}

	public String[] getOptions() {
		return new String[] { "GET", "POST" };
	}

}
