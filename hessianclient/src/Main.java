
import java.net.MalformedURLException;
import java.net.URISyntaxException;

public class Main {

    private static class SystemOutLogger implements Logger {

        public void log(int size, double delay) {
            System.out.println(size + "\t" + delay);
        }
    }

    public static void main(String[] args) throws MalformedURLException,
            URISyntaxException, ClassNotFoundException {
        SystemOutLogger l = new SystemOutLogger();
        if (args.length < 4) {
            System.err.println("usage: java Main <url>"
                    + " <interval> <iterations> <maximum size>\n");
            return;
        }

        //        AxisClient al = new AxisClient(1, 2000,
        //                "http://localhost:8080/axis/AxisTransferService.jws");
        HessianClient hl = new HessianClient(Integer.parseInt(args[2]), Integer
                .parseInt(args[3]), args[0], Integer.parseInt(args[1]));
        //        BurlapClient bl = new BurlapClient(1, 2000,
        //                "http://localhost:8080/hessiantest/burlaptransfer");

        hl.sendMap(l);
    }
}
