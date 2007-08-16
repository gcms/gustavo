package hessian;

import base.ErrorHandler;
import base.RequestLogger;

public class HessianStringRequest extends HessianRequest {

	public HessianStringRequest(long size) {
		super(size);
	}

	public void doRequest(HessianConnection conn, HessianState state,
			RequestLogger logger, ErrorHandler handler) {
		conn.getTransfer().getString(getSize());
	}

	public String toString() {
		return "String(" + getSize() + ")";
	}
}
