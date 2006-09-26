package br.ufg.inf.sd2006;

import java.io.Serializable;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class ObjectReference implements Serializable {
	private static final long serialVersionUID = 9105145520134004733L;

	private InetAddress addr;

	private int port;

	private int oid;

	ObjectReference(InetAddress addr, int port) {
		this(addr, port, newOid());
	}

	public ObjectReference(String hostname, int port, int oid)
			throws UnknownHostException {
		this(InetAddress.getByName(hostname), port, oid);
	}

	public ObjectReference(InetAddress addr, int port, int oid) {
		this.addr = addr;
		this.port = port;
		this.oid = oid;
	}

	public InetAddress getInetAddress() {
		return addr;
	}

	public int getPort() {
		return port;
	}

	public int getOid() {
		return oid;
	}

	private static int oidCount = 0;

	private static int newOid() {
		return ++oidCount;
	}

	/*
	 * public boolean equals(Object o) { if (!(o instanceof ObjectReference))
	 * return false;
	 * 
	 * ObjectReference ref = (ObjectReference) o;
	 * 
	 * return ref.addr.equals(addr) && ref.port == port && ref.oid == oid; }
	 * 
	 * public int hashCode() { return addr.hashCode() + port + oid; }
	 */
	/*
	 * TODO: come back to the previous implementations of equals() and
	 * hashCode(). And implement the desired behaviour by using appropriate
	 * Comparator strategy in the ObjectManager
	 */

	public boolean equals(Object o) {
		if (!(o instanceof ObjectReference))
			return false;

		ObjectReference ref = (ObjectReference) o;

		return ref.port == port && ref.oid == oid;
	}

	public int hashCode() {
		return port + oid;
	}

	public String toString() {
		return "Object " + oid + "\t" + addr + "\t" + port;
	}
}
