package br.ufg.inf.compiler.syntatic;

public class Terminal extends Symbol {
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

}
