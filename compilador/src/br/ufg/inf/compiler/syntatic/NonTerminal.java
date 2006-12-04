package br.ufg.inf.compiler.syntatic;

import java.util.HashSet;
import java.util.Set;

/**
 * Representa um s�mbolo n�o-terminal na gram�tica.
 * 
 * @author gustavo
 *
 */
public class NonTerminal extends Symbol {
	/** Pool de strings j� utilizadas. */
	private static Set<String> stringPool = new HashSet<String>();

	/** S�mbolo representando este n�o-terminal. */
	public String name;

	/**
	 * Cria um n�o-terminal.
	 * 
	 * @param string
	 *            representando o s�mbolo deste n�o-termianl
	 */
	public NonTerminal(String string) {
		name = string;
	}

	/**
	 * Cria um n�o-terminal. Seu s�mbolo � gerado aleatoriamente, usando um
	 * s�mbolo ainda n�o utilizado.
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
	 * Retorna o nome deste n�o-terminal.
	 * 
	 * @return nome do n�o-terminal
	 */
	public String getName() {
		return name;
	}

	/**
	 * Representa��o textual deste n�o terminal.
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
