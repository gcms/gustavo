package base;

public interface Request {
    public void request(Connection conn, State state,
            RequestLogger logger, ErrorHandler handler)
            throws IllegalStateException, java.io.IOException;

    public long getSize();
}