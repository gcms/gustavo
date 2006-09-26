package br.ufg.inf.sd2006;
import java.io.IOException;
import java.net.Socket;

public class RemoteObject {
	private ObjectReference objectReference;

	public RemoteObject(ObjectReference ref) {
		objectReference = ref;
	}

	public Object doOperation(String messageName, Object[] parameters)
			throws IOException, ClassNotFoundException, CommunicationException {
		Request request = new Request(objectReference, messageName, parameters);
		
//		System.out.println("connecting to " + objectReference);
		Socket s = new Socket(objectReference.getInetAddress(), objectReference.getPort());
		ClientConnection conn = new SocketConnection(s);

		Reply reply = conn.sendRequest(request);
		s.close();

		if (request.getRequestId() != reply.getRequestId()) {
			throw new CommunicationException("RequestIDs don't match");
		}
		
		if (reply.getResult() instanceof CommunicationException)
			throw (CommunicationException) reply.getResult();
		
		return reply.getResult();
	}
}
