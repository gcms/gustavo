package base;

public interface RequestLogger {
    public void log(Request req, long start, long end);
}