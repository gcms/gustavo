package br.ufg.inf.compiler.syntatic;

import java.util.HashSet;
import java.util.Set;

public class NonTerminal extends Symbol {
	private static Set<String> stringPool = new HashSet<String>();

	public String text;

	public NonTerminal(String string) {
		text = string;
	}

	public NonTerminal() {
		String random;
		do {
			random = (char) ('a' + (char) (Math.random() * 23))
					+ Integer.toString((int) (Math.random() * 20));
		} while (stringPool.contains(random));

		text = random;
	}

	public String toString() {
		return text;
	}

	public boolean equals(Object o) {
		if (!(o instanceof NonTerminal))
			return false;

		NonTerminal n = (NonTerminal) o;

		return text.equals(n.text);
	}

	public int hashCode() {
		return text.hashCode();
	}

}
