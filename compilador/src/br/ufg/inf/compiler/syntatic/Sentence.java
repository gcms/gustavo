package br.ufg.inf.compiler.syntatic;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

public class Sentence implements Iterable<Symbol>, Collection<Symbol> {

	private List<Symbol> symbols;

	public Sentence(Sentence sentence) {
		this.symbols = new ArrayList<Symbol>(symbols);
	}

	public Sentence(String sentence) {
		symbols = new ArrayList<Symbol>();

		for (int i = 0; i < sentence.length(); i++) {
			char ch = sentence.charAt(i);
			if (Character.isUpperCase(ch)) {
				symbols.add(new NonTerminal(Character.toString(ch)));
			} else if (Character.isLowerCase(ch)) {
				symbols.add(new Terminal(Character.toString(ch)));
			} else {
				throw new IllegalArgumentException();
			}
		}
	}

	public Sentence(Collection<? extends Symbol> symbols) {
		this.symbols = new ArrayList<Symbol>(symbols);		
	}

	public Sentence() {
		this.symbols = new ArrayList<Symbol>();
	}

	public Symbol first() {
		return symbols.get(0);
	}

	public Iterator<Symbol> iterator() {
		return symbols.iterator();
	}

	public boolean add(Symbol e) {
		return symbols.add(e);
	}

	public boolean addAll(Collection<? extends Symbol> c) {
		return symbols.addAll(c);
	}

	public void clear() {
		symbols.clear();
	}

	public boolean contains(Object o) {
		return symbols.contains(o);
	}

	public boolean containsAll(Collection<?> c) {
		return symbols.containsAll(c);
	}

	public boolean isEmpty() {
		return symbols.isEmpty();
	}

	public boolean remove(Object o) {
		return symbols.remove(o);
	}

	public boolean removeAll(Collection<?> c) {
		return symbols.removeAll(c);
	}

	public boolean retainAll(Collection<?> c) {
		return symbols.retainAll(c);
	}

	public int size() {
		return symbols.size();
	}

	public Object[] toArray() {
		return symbols.toArray();
	}

	public <T> T[] toArray(T[] a) {
		return symbols.toArray(a);
	}

	public Collection<? extends Symbol> subSentence(int i, int j) {
		return new Sentence(symbols.subList(i, j));
	}

	public Sentence subSentenceAfter(Symbol s) {
		for (int i = 0; i < size(); i++) {
			if (symbols.get(i).equals(s)) {
				return new Sentence(subSentence(i + 1, symbols.size()));
			}
		}

		return null;
	}
	
	public void removeFirst() {
		symbols.remove(0);
	}
	
	public String toString() {
		return symbols.toString();
	}
}
