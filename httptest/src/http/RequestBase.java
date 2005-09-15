package http;

import java.io.InputStream;

import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.HttpStatus;

import base.Connection;
import base.ErrorHandler;
import base.Request;
import base.RequestLogger;
import base.State;

public abstract class RequestBase implements Request {
    private String path;

    protected void setPath(String path) {
        this.path = path;
    }

    protected String getPath() {
        return path;
    }

    protected void handleResponseBody(InputStream in)
            throws java.io.IOException, IllegalStateException {
        while (in != null && in.read() != -1)
            ;
    }

    public void request(Connection conn, State state, RequestLogger logger,
            ErrorHandler handler) throws IllegalStateException,
            java.io.IOException {
        if (conn == null || !conn.isOpen()) {
            throw new IllegalStateException("Invalid Connection");
        }
        if (state == null) {
            throw new IllegalStateException("Invalid State");
        }

        if (getHttpMethod() == null || !getHttpMethod().validate()) {
            throw new IllegalStateException("Invalid Method");
        }

        long start = System.currentTimeMillis();
        try {
            HttpConnectionConnection httpConn = (HttpConnectionConnection) conn;
            httpConn.request(this, (HttpStateState) state);

            if (getHttpMethod().getStatusCode() != HttpStatus.SC_OK) {
                throw new IllegalArgumentException("Invalid server");
            }

            InputStream in = getHttpMethod().getResponseBodyAsStream();
            handleResponseBody(in);
        } catch (java.io.IOException e) {
            logger.log(this, -1, -1);
            throw e;
        } catch (IllegalStateException e) {
            logger.log(this, -1, -1);
            throw e;
        }
        long end = System.currentTimeMillis();

        logger.log(this, start, end);
    }

    public abstract HttpMethod getHttpMethod();

    public String toString() {
        return super.toString() + "\t" + getSize();
    }
}