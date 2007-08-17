package br.ufg.inf.crawler;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.List;

public class Manager {
    private WordIndex index = new WordIndex();

    private URLPool pool = new URLPool();

    public void handle(URL url, Resource resource) {
	index.index(url, resource);
	pool.addLinks(resource.getLinks());
	pool.update(url);
    }

    public URL nextURL() {
	return pool.nextURL();
    }

    public List<URL> search(String word) {
	return search(word.split("\\s"));
    }

    public List<URL> search(String... words) {
	return index.getResources(words);
    }

    public void start() throws InterruptedException {
	while (true) {
	    new Thread(new WorkThread(this)).start();
	    Thread.sleep(60 * 1000);
	}
    }

    public void index(URL url) {
	pool.addLink(url);
    }

    public void index(String string) throws MalformedURLException {
	pool.addLink(new URL(string));
    }

    public void invalidate(URL url) {
	pool.update(url);
    }
}
