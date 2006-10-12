package br.ufg.inf.sd2006.example;

import java.net.UnknownHostException;

import br.ufg.inf.sd2006.Binder;
import br.ufg.inf.sd2006.ConcreteBinder;
import br.ufg.inf.sd2006.RemoteObject;

public class ClientMain3 {

    private static interface Somador {
	public Integer soma(Integer a, Integer b);
    }

    public static void main(String[] args) throws UnknownHostException {
	Binder b = ConcreteBinder.getBinder("localhost", 9000);

	Somador s = (Somador) new RemoteObject(b.lookup("somador"))
		.getProxy(Somador.class);

	System.out.println(s.soma(10, 2));
    }

}
