package br.ufg.inf.compiler.syntatic;

/**
 * Representa um símbolo terminal na gramática.
 * 
 * @author gustavo
 * 
 */
public class Terminal extends Symbol {
	/** Constante representando cadeia vazia. */
	public static final Terminal EPSILON = new Terminal("EPSILON");

	/** Constante representand fim da cadeia. */
	public static final Terminal END = new Terminal("$");

	/** Nome do terminal. */
	public String name;

	/**
	 * Cria um terminal.
	 * 
	 * @param string
	 */
	public Terminal(String string) {
		name = string;
	}

	/**
	 * Obtém nome do terminal.
	 * @return nome do terminal
	 */
	public String getName() {
		return name;
	}

	/**
	 * Obtém string representando terminal.
	 * @return string do terminal
	 */
	public String toString() {
		return name;
	}

	@Override
	public int hashCode() {
		final int PRIME = 31;
		int result = 1;
		result = PRIME * result + ((name == null) ? 0 : name.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		final Terminal other = (Terminal) obj;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}

}
