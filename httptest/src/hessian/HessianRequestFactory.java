package hessian;

import base.Request;
import base.RequestFactory;

public class HessianRequestFactory implements RequestFactory {

	public Request getRequest(String param) {
		return null;
	}

	public Request getRequest() {
		return null;
	}

	public Request getRequest(String param, long size) {
		return param.equals("String") ? (Request) new HessianStringRequest(size)
				: param.equals("Map") ? (Request) new HessianMapRequest(size)
						: null;
	}

	public String[] getOptions() {
		return new String[] { "String", "Map" };
	}

}
