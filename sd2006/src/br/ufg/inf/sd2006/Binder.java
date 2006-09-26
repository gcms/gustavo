package br.ufg.inf.sd2006;

public interface Binder {

	public void bind(String name, ObjectReference ref);

	public void bind(String name, Object o);

	public ObjectReference lookup(String name);

}
