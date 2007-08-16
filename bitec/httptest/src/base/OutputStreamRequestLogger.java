package base;

public class OutputStreamRequestLogger implements RequestLogger {
    private java.io.PrintStream out;

    public OutputStreamRequestLogger(java.io.OutputStream out) {
        this.out = new java.io.PrintStream(out);
    }

    public void log(Request req, long start, long end) {
        out.println(req + "\t" + start + "\t" + end);
    }
}