package br.ufg.inf.sd2006;

import java.io.Serializable;

public class Request implements Serializable{
	private static final long serialVersionUID = -7341314590454684889L;

	private int requestId;
	
	private ObjectReference objectRef;
	private String messageName;
	private Object[] params;
	
	public Request(ObjectReference objectRef, String methodName, Object[] params) {
		this.objectRef = objectRef;
		this.messageName = methodName;
		this.params = params;
		
		requestId = registerRequest();
	}
	
	private static int requestCount = 0;	

	private int registerRequest() {
		return ++requestCount;
	}

	public String getMethodName() {
		return messageName;
	}

	public ObjectReference getObjectRef() {
		return objectRef;
	}

	public Object[] getParams() {
		return params;
	}

	public int getRequestId() {
		return requestId;
	}
}
