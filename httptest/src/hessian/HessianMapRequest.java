package hessian;

import base.ErrorHandler;
import base.RequestLogger;

public class HessianMapRequest extends HessianRequest {

	public HessianMapRequest(long size) {
		super(size);
	}

	public void doRequest(HessianConnection conn, HessianState state,
			RequestLogger logger, ErrorHandler handler) {
		conn.getTransfer().getMap(getSize());
	}

	public String toString() {
		return "Map(" + getSize() + ")";
	}
}
