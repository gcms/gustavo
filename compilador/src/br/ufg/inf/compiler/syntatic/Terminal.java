package br.ufg.inf.compiler.syntatic;

public class Terminal extends Symbol {
	public static final Terminal EPSILON = new Terminal("EPSILON");

	public static final Terminal END = new Terminal("$");

	public String name;

	public String value;

	public Terminal(String string) {
		name = string;
	}

	public Terminal(String name, String value) {
		this(name);
		this.value = value;
	}

	public String toString() {
		return name;
	}

	public boolean equals(Object o) {
		if (!(o instanceof Terminal))
			return false;

		Terminal n = (Terminal) o;

		return name.equals(n.name);
	}

	public int hashCode() {
		return name.hashCode();
	}
}
