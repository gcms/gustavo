package br.ufg.inf.crawler;
import java.io.IOException;
import java.net.URL;

public class WorkThread implements Runnable {
    private static final long MAXIMUM_TIME = 30 * 1000;

    private long maxTime = MAXIMUM_TIME;

    private Manager manager;

    public WorkThread(Manager manager) {
	this.manager = manager;
    }

    public void run() {
	long startTime = System.currentTimeMillis();

	while (true) {
	    URL url = manager.nextURL();

	    if (url == null)
		continue;

	    Resource resource;
	    try {
		System.out.println("parsing " + url);
		resource = Parser.getResource(url);
	    } catch (IOException e) {
		manager.invalidate(url);
//		e.printStackTrace();
		continue;
	    }

	    manager.handle(url, resource);

	    if (System.currentTimeMillis() - startTime > maxTime)
		break;
	}
    }
}