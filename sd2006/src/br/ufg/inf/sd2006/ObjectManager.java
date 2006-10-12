package br.ufg.inf.sd2006;

import java.net.InetAddress;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

import java.util.Map;

public class ObjectManager {
    private Map<Object, ObjectReference> objectToRef;

    private Map<ObjectReference, Object> refToObject;

    private InetAddress addr;

    private int port;

    public ObjectManager(InetAddress addr, int port) {
	objectToRef = new HashMap<Object, ObjectReference>();
	refToObject = new HashMap<ObjectReference, Object>();

	this.addr = addr;
	this.port = port;

	registerObject(new ObjectReference(addr, port, 0), this);
	registerObject(new ObjectReference(addr, port, -1), new ConcreteBinder(
		this));
    }

    private void registerObject(ObjectReference ref, Object o) {
	objectToRef.put(o, ref);
	refToObject.put(ref, o);
    }

    public ObjectReference registerObject(Object o) {
	ObjectReference ref = new ObjectReference(addr, port);

	System.out.println("registering " + ref + " to " + o);

	objectToRef.put(o, ref);
	refToObject.put(ref, o);

	return ref;
    }

    public Object getObject(ObjectReference ref) {
	System.out.println("searching for " + ref);
	return refToObject.get(ref);
    }

    public ObjectReference getObjectReference(Object o) {
	return objectToRef.get(o);
    }

    public Set<ObjectReference> listObjectReferences() {
	return new HashSet<ObjectReference>(refToObject.keySet());
    }

    public ConcreteBinder getBinder() {
	return (ConcreteBinder) getObject(new ObjectReference(addr, port, -1));
    }
}
