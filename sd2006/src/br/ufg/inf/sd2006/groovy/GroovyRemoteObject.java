package br.ufg.inf.sd2006.groovy;

import groovy.util.Proxy;

import java.net.UnknownHostException;

import br.ufg.inf.sd2006.ObjectReference;
import br.ufg.inf.sd2006.RemoteObject;

public class GroovyRemoteObject extends Proxy {
    private RemoteObject obj;

    public GroovyRemoteObject(RemoteObject obj) {
	this.obj = obj;
    }

    public GroovyRemoteObject(ObjectReference ref) {
	this(new RemoteObject(ref));
    }

    public GroovyRemoteObject(String host, int port, int oid)
	    throws UnknownHostException {
	this(new ObjectReference(host, port, oid));
    }

    public Object invokeMethod(String name, Object args)
	    throws RuntimeException {
	try {
	    Object result = obj.doOperation(name, (Object[]) args);
	    if (result instanceof ObjectReference)
		result = new GroovyRemoteObject((ObjectReference) result);

	    return result;
	} catch (Exception e) {
	    throw new RuntimeException(e);
	}
    }
}
