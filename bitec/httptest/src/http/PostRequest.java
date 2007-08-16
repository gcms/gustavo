package http;

import org.apache.commons.httpclient.HttpMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.methods.multipart.FilePart;
import org.apache.commons.httpclient.methods.multipart.MultipartRequestEntity;
import org.apache.commons.httpclient.methods.multipart.Part;

public class PostRequest extends RequestBase {
    private PostMethod method;

    private long size;

    public PostRequest(long size) {
        this.size = size;
        method = new PostMethod();

        Part[] p = { new FilePart("data", new StubPartSource("data", size)) };

        method.setRequestEntity(new MultipartRequestEntity(p, method
                .getParams()));
    }

    public HttpMethod getHttpMethod() {
        method.setPath(getPath() + "?nbytes=" + size);
        return method;
    }

    public long getSize() {
        return size;
    }

    public String toString() {
        return "POST(" + getSize() + ")";
    }
}