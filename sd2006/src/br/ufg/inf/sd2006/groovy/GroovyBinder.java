package br.ufg.inf.sd2006.groovy;

import java.net.UnknownHostException;

public class GroovyBinder extends GroovyRemoteObject {
    public GroovyBinder(String host, int port) throws UnknownHostException {
	super(host, port, -1);
    }
}
