package br.ufg.inf.sd2006.groovy;

import groovy.util.Proxy;

import java.net.UnknownHostException;

import br.ufg.inf.sd2006.ObjectReference;
import br.ufg.inf.sd2006.RemoteObject;

public class GroovyRuntime {
	public static Proxy getGroovy(final RemoteObject o) {
		return new Proxy() {
			public Object invokeMethod(String name, Object args)
					throws RuntimeException {
				try {
					return o.doOperation(name, (Object[]) args);
				} catch (Exception e) {
					throw new RuntimeException(e);
				}
			}
		};
	}

	public static Proxy getBinder(String host, int port)
			throws UnknownHostException {
		final RemoteObject o = new RemoteObject(new ObjectReference(host, port,
				-1));
		return new Proxy() {
			public Object invokeMethod(String name, Object args)
					throws RuntimeException {
				try {
					return o.doOperation(name, (Object[]) args);
				} catch (Exception e) {
					throw new RuntimeException(e);
				}
			}
		};

	}
}
