package br.ufg.inf.sd2006.example;

import java.io.IOException;
import java.util.List;
import java.util.Set;

import br.ufg.inf.sd2006.CommunicationException;
import br.ufg.inf.sd2006.ObjectReference;
import br.ufg.inf.sd2006.RemoteObject;

public class ClientMain {
	@SuppressWarnings("unchecked")
	public static void main(String[] args) throws IOException,
			ClassNotFoundException, CommunicationException {
		ObjectReference ref = new ObjectReference("localhost", 9000, 1);

		RemoteObject obj = new RemoteObject(ref);

		/* chama metodo list na classe Lista */
		List l = (List) obj.doOperation("list", new Object[] {});

		System.out.println(l);

		obj = new RemoteObject(new ObjectReference("localhost", 9000, 2));
		Integer i = (Integer) obj.doOperation("soma", new Object[] { 3, 4 });
		System.out.println(i);

		obj = new RemoteObject(new ObjectReference("localhost", 9000, 0));
		Set<ObjectReference> doOperation = (Set<ObjectReference>) obj.doOperation(
						"listObjectReferences", new Object[] {});
		Set<ObjectReference> refs = doOperation;

		System.out.println(refs);
		for (ObjectReference r : refs) {
			Class c = (Class) (new RemoteObject(r).doOperation("getClass",
					new Object[] {}));
			
			System.out.println(c.getName());
		}
	}
}
