package hessian;

import java.net.URI;

import base.Connection;
import base.ConnectionFactory;

public class HessianConnectionFactory implements ConnectionFactory {
	
	private URI uri;
	
	public HessianConnectionFactory(URI uri) {
		this.uri = uri;
	}

	public Connection getConnection() {
		return new HessianConnection(uri);
	}

}
