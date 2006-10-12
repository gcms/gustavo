package br.ufg.inf.sd2006.example;

import java.io.IOException;

import br.ufg.inf.sd2006.Binder;
import br.ufg.inf.sd2006.ConcreteBinder;
import br.ufg.inf.sd2006.CommunicationException;
import br.ufg.inf.sd2006.RemoteObject;

public class ClientMain2 {
    public static void main(String[] args) throws IOException,
	    ClassNotFoundException, CommunicationException {
	Binder b = ConcreteBinder.getBinder("localhost", 9000);

	RemoteObject obj = new RemoteObject(b.lookup("somador"));
	Integer i = (Integer) obj.doOperation("soma", new Object[] { 1, 6 });

	System.out.println(i);

    }
}
