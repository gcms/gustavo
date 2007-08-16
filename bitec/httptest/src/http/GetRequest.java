package http;

import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.methods.GetMethod;

import java.io.InputStream;

public class GetRequest extends RequestBase {
    private GetMethod method;

    private long size;

    public GetRequest(long size) {
        method = new GetMethod();
        this.size = size;
    }

    public long getSize() {
        return size;
    }

    public HttpMethod getHttpMethod() {
        method.setPath(getPath() + "?nbytes=" + size);
        return method;
    }

    protected void handleResponseBody(InputStream in)
            throws java.io.IOException, IllegalStateException {
        long bytesRead = 0;
        while (in.read() != -1)
            bytesRead++;

        if (bytesRead != size) {
            System.out.println(bytesRead);
            throw new IllegalStateException("Invalid server");
        }
    }

    public String toString() {
        return "GET(" + getSize() + ")";
    }
}