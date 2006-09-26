package br.ufg.inf.sd2006.example;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import br.ufg.inf.sd2006.ConcreteBinder;
import br.ufg.inf.sd2006.ObjectManager;
import br.ufg.inf.sd2006.SDServer;

public class ServerMain {

	public static class Lista {
		public Object list() {
			List<String> l = new ArrayList<String>();
			l.add("Somador");

			return l;
		}
	}

	public static class Somador {
		public Integer soma(Integer a, Integer b) {
			return a + b;
		}
	}

	public static void main(String[] args) throws IOException {
		SDServer server = new SDServer(9000);

		ObjectManager manager = server.getObjectManager();
		ConcreteBinder binder = manager.getBinder();

		binder.bind("lista", manager.registerObject(new Lista()));
		binder.bind("somador", manager.registerObject(new Somador()));

		server.run();
	}
}