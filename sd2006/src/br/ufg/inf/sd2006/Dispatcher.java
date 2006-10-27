package br.ufg.inf.sd2006;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Dispatcher {
    private ObjectManager manager;

    public Dispatcher(ObjectManager manager) {
	this.manager = manager;
    }

    public void dispatch(ServerConnection conn) throws IOException {

	System.out.println("Waiting for request from " + conn);
	Request request = conn.getRequest();
	System.out.println("request on " + request.getObjectRef());

	Object[] params = request.getParams();
	Class[] paramClass = null;

	if (params != null) {
	    paramClass = new Class[params.length];

	    for (int i = 0; i < params.length; i++) {
		paramClass[i] = params[i].getClass();
	    }
	}

	Object obj = manager.getObject(request.getObjectRef());
	if (obj == null) {
	    conn.sendReply(new Reply(request, new CommunicationException(
		    "Object not found")));
	    return;
	}

	Method m;
	try {
	    m = obj.getClass().getMethod(request.getMethodName(), paramClass);
	} catch (NoSuchMethodException e) {
	    conn.sendReply(new Reply(request, new CommunicationException(
		    "Method not found")));
	    e.printStackTrace();
	    return;
	}

	try {
	    conn.sendReply(new Reply(request, m.invoke(obj, params)));
	    return;
	} catch (IllegalArgumentException e) {
	    conn.sendReply(new Reply(request, new CommunicationException(
		    "Illegal arguments")));
	    e.printStackTrace();
	    return;
	} catch (IllegalAccessException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	    conn.sendReply(new Reply(request, new CommunicationException(e)));
	    return;
	} catch (InvocationTargetException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	    conn.sendReply(new Reply(request, new CommunicationException(e)));
	    return;
	}

    }

}