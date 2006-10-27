package br.ufg.inf.sd2006;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class SDServer implements Runnable {
    public class ClientRunner implements Runnable {

	private Socket client;

	public ClientRunner(Socket client) {
	    this.client = client;
	}

	public void run() {
	    try {
		dispatcher.dispatch(new SocketConnection(client));
		client.close();
	    } catch (IOException e) {
		e.printStackTrace();
	    }
	}
    }

    private ServerSocket server;

    private ObjectManager manager;

    private Dispatcher dispatcher;

    public SDServer(ObjectManager manager, ServerSocket server) {
	this.manager = manager;
	this.server = server;
	this.dispatcher = new Dispatcher(manager);
    }

    public SDServer(int port) throws IOException {
	server = new ServerSocket(port);
	manager = new ObjectManager(server.getInetAddress(), server
		.getLocalPort());
	dispatcher = new Dispatcher(manager);
    }

    public ObjectManager getObjectManager() {
	return manager;
    }

    public void run() {
	while (true) {
	    Socket client;
	    try {
		client = server.accept();
	    } catch (IOException e) {
		e.printStackTrace();
		continue;
	    }
	    System.out.println("Accepted connection from " + client);

	    new Thread(new ClientRunner(client)).start();

	}

    }

}
