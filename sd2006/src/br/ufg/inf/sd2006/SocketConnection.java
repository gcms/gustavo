package br.ufg.inf.sd2006;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class SocketConnection implements ClientConnection, ServerConnection {
    private Socket socket;

    public SocketConnection(Socket socket) {
	this.socket = socket;
    }

    public Reply sendRequest(Request req) throws IOException {

	// System.out.println("sendRequest()");
	ObjectOutputStream oos = new ObjectOutputStream(socket
		.getOutputStream());

	oos.writeObject(req);
	oos.flush();
	// System.out.println("Request written");

	ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
	// System.out.println("reading reply");
	Reply r;
	try {
	    r = (Reply) ois.readObject();
	} catch (ClassNotFoundException e) {
	    e.printStackTrace();

	    return null;
	}

	// System.out.println("Reply read");

	return r;
    }

    public Request getRequest() throws IOException {
	ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());

	Request req;
	try {
	    // System.out.println("ois.readObject()");
	    req = (Request) ois.readObject();
	} catch (ClassNotFoundException e) {
	    e.printStackTrace();
	    return null;
	}
	return req;
    }

    public void sendReply(Reply reply) throws IOException {
	ObjectOutputStream oos = new ObjectOutputStream(socket
		.getOutputStream());

	oos.writeObject(reply);
    }

}
