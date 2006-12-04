package br.ufg.inf.compiler.syntatic;

import java.util.HashSet;
import java.util.Set;

/**
 * Representa um símbolo não-terminal na gramática.
 * 
 * @author gustavo
 *
 */
public class NonTerminal extends Symbol {
	/** Pool de strings já utilizadas. */
	private static Set<String> stringPool = new HashSet<String>();

	/** Símbolo representando este não-terminal. */
	public String name;

	/**
	 * Cria um não-terminal.
	 * 
	 * @param string
	 *            representando o símbolo deste não-termianl
	 */
	public NonTerminal(String string) {
		name = string;
	}

	/**
	 * Cria um não-terminal. Seu símbolo é gerado aleatoriamente, usando um
	 * símbolo ainda não utilizado.
	 * 
	 */
	public NonTerminal() {
		String random;
		do {
			random = (char) ('a' + (char) (Math.random() * 23))
					+ Integer.toString((int) (Math.random() * 20));
		} while (stringPool.contains(random));

		name = random;
	}

	/**
	 * Retorna o nome deste não-terminal.
	 * 
	 * @return nome do não-terminal
	 */
	public String getName() {
		return name;
	}

	/**
	 * Representação textual deste não terminal.
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
		final NonTerminal other = (NonTerminal) obj;
		if (name == null) {
			if (other.name != null)
				return false;
		} else if (!name.equals(other.name))
			return false;
		return true;
	}

}
