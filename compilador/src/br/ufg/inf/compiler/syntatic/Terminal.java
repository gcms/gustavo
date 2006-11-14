package br.ufg.inf.compiler.syntatic;

public class Terminal extends Symbol {
	public static final Terminal EPSILON = new Terminal("EPSILON");

	public static final Terminal END = new Terminal("$");

	public String text;

	public Terminal(String string) {
		text = string;
	}

	public String toString() {
		return text;
	}

	public boolean equals(Object o) {
		if (!(o instanceof Terminal))
			return false;

		Terminal n = (Terminal) o;

		return text.equals(n.text);
	}

	public int hashCode() {
		return text.hashCode();
	}
}
