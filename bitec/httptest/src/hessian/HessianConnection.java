package hessian;

import java.io.IOException;
import java.net.URI;

import com.caucho.hessian.client.HessianProxyFactory;

import base.Connection;

public class HessianConnection implements Connection {

	private HessianProxyFactory factory;

	private Transfer transfer;

	private URI uri;

	public HessianConnection(URI uri) {
		this.uri = uri;
	}

	public boolean isOpen() {
		return true;
	}

	public void open() throws IOException {
		factory = new HessianProxyFactory();
		transfer = (Transfer) factory.create(Transfer.class, uri
				.toString());
	}

	public Transfer getTransfer() {
		return transfer;
	}

	public void close() {
		transfer = null;
		factory = null;
	}
}
