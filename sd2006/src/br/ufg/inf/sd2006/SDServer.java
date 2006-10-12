package br.ufg.inf.sd2006;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.ServerSocket;
import java.net.Socket;

public class SDServer implements Runnable {
    public static class ClientRunner implements Runnable {
	private ObjectManager manager;

	private Socket client;

	public ClientRunner(ObjectManager manager, Socket client) {
	    this.manager = manager;
	    this.client = client;
	}

	public void run() {
	    try {
		doRun();
		client.close();
	    } catch (IOException e) {
		e.printStackTrace();
	    }
	}

	public void doRun() throws IOException {
	    ServerConnection conn = new SocketConnection(client);

	    System.out.println("Waiting for request from " + client);
	    Request request = conn.getRequest();
	    System.out.println("request on " + request.getObjectRef());

	    Object[] params = request.getParams();
	    Class[] paramClass = new Class[params.length];

	    for (int i = 0; i < params.length; i++) {
		paramClass[i] = params[i].getClass();
	    }

	    Object obj = manager.getObject(request.getObjectRef());
	    if (obj == null) {
		conn.sendReply(new Reply(request, new CommunicationException(
			"Object not found")));
		return;
	    }

	    Method m;
	    try {
		m = obj.getClass().getMethod(request.getMethodName(),
			paramClass);
	    } catch (NoSuchMethodException e) {
		conn.sendReply(new Reply(request, new CommunicationException(
			"Method not found")));
		e.printStackTrace();
		return;
	    }

	    try {
		conn.sendReply(new Reply(request, m.invoke(obj, params)));
		return;
	    } catch (IllegalArgumentException e) {
		conn.sendReply(new Reply(request, new CommunicationException(
			"Illegal arguments")));
		e.printStackTrace();
		return;
	    } catch (IllegalAccessException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		conn
			.sendReply(new Reply(request,
				new CommunicationException(e)));
		return;
	    } catch (InvocationTargetException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		conn
			.sendReply(new Reply(request,
				new CommunicationException(e)));
		return;
	    }

	}

    }

    private ServerSocket server;

    private ObjectManager manager;

    public SDServer(ObjectManager manager, ServerSocket server) {
	this.manager = manager;
	this.server = server;
    }

    public SDServer(int port) throws IOException {
	server = new ServerSocket(port);
	manager = new ObjectManager(server.getInetAddress(), server
		.getLocalPort());
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

	    new Thread(new ClientRunner(manager, client)).start();
	}

    }

}
