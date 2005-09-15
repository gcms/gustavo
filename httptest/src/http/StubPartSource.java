package http;

import org.apache.commons.httpclient.methods.multipart.PartSource;

public class StubPartSource implements PartSource {
    private long length;

    private String fileName;

    public StubPartSource(String fileName, long length)
            throws IllegalArgumentException {
        if (length < 0) {
            throw new IllegalArgumentException();
        }
        this.length = length;
        this.fileName = fileName;
    }

    private static class StubInputStream extends java.io.InputStream {
        private long length;

        public StubInputStream(long length) {
            this.length = length;
        }

        public int available() {
            return (int) length;
        }

        public int read() {
            if (available() <= 0) {
                return -1;
            }
            return (int) length--;
        }
    }

    public java.io.InputStream createInputStream() {
        return new StubInputStream(length);
    }

    public long getLength() {
        return length;
    }

    public String getFileName() {
        return fileName;
    }
}