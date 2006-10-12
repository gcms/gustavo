package br.ufg.inf.sd2006;

import java.io.Serializable;

public class Reply implements Serializable {
    private static final long serialVersionUID = -2205952221950468139L;

    private int requestId;

    private Object result;

    public Reply(Request req, Object result) {
	this.result = result;

	requestId = req.getRequestId();
    }

    public int getRequestId() {
	return requestId;
    }

    public Object getResult() {
	return result;
    }
}
