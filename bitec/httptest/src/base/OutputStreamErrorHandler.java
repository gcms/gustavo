package base;

public class OutputStreamErrorHandler implements ErrorHandler {
    private java.io.PrintStream out;

    public OutputStreamErrorHandler(java.io.OutputStream stream) {
        out = new java.io.PrintStream(stream);
    }

    public void debug(Object o) {
        out.println("ERROR\t" + o);
    }
}