package br.ufg.inf.crawler;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
    public static void main(String[] args) throws IOException,
	    InterruptedException {
	final Manager manager = new Manager();
	manager.index("http://www.google.com/");
	manager.index("http://www.uol.com.br/");
	manager.index("http://www.terra.com.br/");

	new Thread() {
	    public void run() {
		try {
		    manager.start();
		} catch (InterruptedException e) {
		    // TODO Auto-generated catch block
		    e.printStackTrace();
		}
	    }
	}.start();

	BufferedReader reader = new BufferedReader(new InputStreamReader(
		System.in));

	String line;
	while ((line = reader.readLine()) != null) {
	    System.out.println(manager.search(line));
	}
    }
}
