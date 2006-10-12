package br.ufg.inf.sd2006;

import java.io.IOException;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.Socket;

public class RemoteObject {
    private ObjectReference objectReference;

    public RemoteObject(ObjectReference ref) {
	if (ref == null) {
	    throw new IllegalArgumentException(
		    "Must have a valid object reference");
	}

	objectReference = ref;
    }

    public Object doOperation(String messageName, Object[] parameters)
	    throws CommunicationException {
	Object result;
	try {
	    result = dispatchOperation(messageName, parameters);
	} catch (IOException e) {
	    e.printStackTrace();
	    throw new CommunicationException(e);
	} catch (ClassNotFoundException e) {
	    e.printStackTrace();
	    throw new CommunicationException(e);
	}

	if (result instanceof CommunicationException)
	    throw (CommunicationException) result;

	return result;
    }

    private Object dispatchOperation(String messageName, Object[] parameters)
	    throws IOException, ClassNotFoundException, CommunicationException {
	Request request = new Request(objectReference, messageName, parameters);

	// System.out.println("connecting to " + objectReference);
	Socket s = new Socket(objectReference.getInetAddress(), objectReference
		.getPort());
	ClientConnection conn = new SocketConnection(s);

	Reply reply = conn.sendRequest(request);
	s.close();

	if (request.getRequestId() != reply.getRequestId()) {
	    throw new CommunicationException("RequestIDs don't match");
	}

	return reply.getResult();
    }

    private static class RemoteObjectProxy implements InvocationHandler {
	private RemoteObject o;

	public RemoteObjectProxy(RemoteObject o) {
	    this.o = o;
	}

	public Object invoke(Object arg0, Method arg1, Object[] arg2)
		throws Throwable {
	    return o.doOperation(arg1.getName(), arg2);
	}
    }

    public Object getProxy(Class[] interfaces) {
	return Proxy.newProxyInstance(RemoteObject.class.getClassLoader(),
		interfaces, new RemoteObjectProxy(this));
    }

    public Object getProxy(Class iface) {
	return getProxy(new Class[] { iface });
    }
}
