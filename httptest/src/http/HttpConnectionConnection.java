package http;

import java.io.IOException;
import java.net.URI;

import org.apache.commons.httpclient.HttpConnection;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.HttpMethod;

import base.Connection;

public class HttpConnectionConnection implements Connection {
    private HttpConnection conn;

    private URI uri;

    public HttpConnectionConnection(URI uri) {
        this.uri = uri;
        conn = new HttpConnection(uri.getHost(), uri.getPort());
    }

    public boolean isOpen() {
        return conn.isOpen();
    }

    public void open() throws IOException {
        conn.open();
    }

    public void close() {
        conn.close();
    }

    public void request(RequestBase req, HttpStateState state)
            throws HttpException, IOException {
        req.setPath(uri.getPath());
        req.getHttpMethod().execute(((HttpStateState) state).getState(), conn);
    }

}