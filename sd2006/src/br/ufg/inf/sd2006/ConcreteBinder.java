package br.ufg.inf.sd2006;

import java.io.Serializable;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.Map;

public class ConcreteBinder implements Binder, Serializable {
	private static final long serialVersionUID = -1939920662560187597L;

	private ObjectManager manager;

	private Map<String, ObjectReference> bindings;

	public ConcreteBinder(ObjectManager manager) {
		this.manager = manager;

		bindings = new HashMap<String, ObjectReference>();

		bind("manager", manager);
	}

	public void bind(String name, ObjectReference ref) {
		bindings.put(name, ref);
	}

	public void bind(String name, Object o) {
		ObjectReference ref = manager.getObjectReference(o);

		if (ref == null)
			throw new IllegalArgumentException("Object is not registered");

		bind(name, ref);
	}

	public ObjectReference lookup(String name) {
		return bindings.get(name);
	}

	private static class BinderProxy implements InvocationHandler {
		private RemoteObject obj;

		public BinderProxy(RemoteObject o) {
			obj = o;
		}

		public Object invoke(Object arg0, Method arg1, Object[] arg2)
				throws Throwable {
			return obj.doOperation(arg1.getName(), arg2);
		}
	}

	public static Binder getBinder(String host, int port)
			throws UnknownHostException {
		{
			BinderProxy p = new BinderProxy(new RemoteObject(
					new ObjectReference(host, port, -1)));
			return (Binder) Proxy.newProxyInstance(ConcreteBinder.class
					.getClassLoader(), new Class[] { Binder.class }, p);
		}
	}

}
