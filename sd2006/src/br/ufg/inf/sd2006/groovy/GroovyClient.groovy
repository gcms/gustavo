package br.ufg.inf.sd2006.groovy;


import br.ufg.inf.sd2006.Binder;
import br.ufg.inf.sd2006.RemoteObject;
import br.ufg.inf.sd2006.ObjectReference;

class GroovyClient {

  static void main(args) {
	def binder = new GroovyBinder("localhost", 9000);
	def somador = binder.lookup("somador");
	
	println somador.soma(10, 2);
	
	def manager = binder.lookup("manager");
	println manager.listObjectReferences();
  }

}
