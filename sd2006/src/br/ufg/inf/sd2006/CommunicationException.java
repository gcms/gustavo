package br.ufg.inf.sd2006;

public class CommunicationException extends Exception {

    private static final long serialVersionUID = -7495876577704486981L;

    public CommunicationException(String msg) {
	super(msg);
    }

    public CommunicationException(Throwable t) {
	super(t);
    }
}
