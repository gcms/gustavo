package http;

import java.net.URI;

import base.Connection;
import base.ConnectionFactory;

public class HttpConnectionConnectionFactory implements ConnectionFactory {
    private URI uri;

    public HttpConnectionConnectionFactory(URI uri) {
        this.uri = uri;
    }

    public Connection getConnection() {
        return new HttpConnectionConnection(uri);
    }
}