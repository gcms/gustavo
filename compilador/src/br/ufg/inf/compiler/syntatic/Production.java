package br.ufg.inf.compiler.syntatic;

import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 * Representa uma produ��o em uma gram�tica.
 * 
 * @author gustavo
 * 
 */
public class Production {
	/** Lado esquerdo da produ��o. */
	private NonTerminal variable;

	/** Lado direito da produ��o. */
	private Sentence sentence;

	/**
	 * Cria uma produ��o
	 * 
	 * @param variable
	 *            vari�vel (lado esquerdo)
	 * @param symbols
	 *            s�mbolos (lado direito)
	 */
	Production(NonTerminal variable, Symbol[] symbols) {
		this.variable = variable;
		this.sentence = new Sentence();
		Collections.addAll(this.sentence, symbols);
	}

	/**
	 * Cria uma produ��o
	 * 
	 * @param variable
	 *            vari�vel (lado esquerdo)
	 * @param symbols
	 *            s�mbolos (lado direito)
	 */
	Production(NonTerminal variable, Sentence symbols) {
		this.variable = variable;
		this.sentence = new Sentence();
		this.sentence = symbols;
	}

	/**
	 * Cria uma produ��o.
	 */
	private Production() {
		variable = null;
		sentence = new Sentence();
	}

	/**
	 * Obt�m o lado direito da produ��o.
	 * 
	 * @return forma sentencial (lado direito)
	 */
	public Sentence getSentence() {
		return sentence;
	}

	/**
	 * Obt�m o lado esquerdo da produ��o
	 * 
	 * @return vari�vel (lado esquerdo)
	 */
	public NonTerminal getVariable() {
		return variable;
	}

	/**
	 * Determina se a produ��o � recursiva.
	 * 
	 * @return true se a produ��o � recursiva
	 */
	public boolean isRecursive() {
		return sentence.contains(variable);
	}

	/**
	 * Retorna o tamanho do lado direito da produ��o
	 * 
	 * @return n�mero de s�mbolos do lado direito da produ��o
	 */
	public int numSymbols() {
		return getSentence().size();
	}

	/**
	 * Determina se a produ��o deriva o s�mbolo s.
	 * 
	 * @param sa
	 *            true se esta produ��o deriva s
	 * @return
	 */
	public boolean derives(Symbol s) {
		return numSymbols() == 1 && getSentence().first().equals(s);
	}

	/**
	 * Determina se a produ��o deriva EPSILON.
	 * 
	 * @return true se esta produ��o deriva EPSILON
	 */
	public boolean derivesEpsilon() {
		return numSymbols() == 0;
	}

	/**
	 * Determina se o lado direito da produ��o cont�m o s�mbolo s.
	 * 
	 * @param s
	 *            s�mbolo a ser procurado
	 * @return se s est� no lado direito da produ��o
	 */
	public boolean hasDerivation(Symbol s) {
		return sentence.contains(s);
	}

	/**
	 * Testa se os s�mbolos desta produ��o s�o um subconjunto do conjunto
	 * especificado.
	 * 
	 * @param s
	 *            conjunto de s�mbolos especificado
	 * @return true se o lado direito � subconjunto de s
	 */
	public boolean symbolsIsSubsetOf(Set<Symbol> s) {
		for (Symbol e : sentence) {
			if (!s.contains(e))
				return false;
		}

		return true;
	}

	/**
	 * Generates all arrangements of symbols removing or not the symbol e from
	 * the symbol list. Its intended to be used in the process of eliminating
	 * Epsilon variables as this behaviour is the same as exchanging the symbol
	 * for Epsilon.
	 * 
	 * @param e
	 *            symbol to be substitued for Epsilon
	 * @return a of productions exchanging one or more occurrences of e for
	 *         Epsilon
	 */
	public Set<Production> remove(Symbol e) {
		Set<Production> result = new HashSet<Production>();
		result.add(this);

		int lastSize;

		do {
			lastSize = result.size();
			Production toAdd = null;

			for (Production p : result) {
				Production clone = (Production) p.clone();
				for (Iterator<Symbol> it = clone.sentence.iterator(); it
						.hasNext(); /* */) {
					Symbol s = it.next();

					if (s.equals(e)) {
						it.remove();
						toAdd = clone;
						break;
					}
				}
			}

			if (toAdd != null)
				result.add(toAdd);

		} while (lastSize != result.size());

		return result;
	}

	/**
	 * Determina se a produ��o cicla.
	 * 
	 * @return true se cicla
	 */
	public boolean isCycle() {
		return numSymbols() == 1 && getSentence().first().equals(variable);
	}

	@Override
	public int hashCode() {
		final int PRIME = 31;
		int result = 1;
		result = PRIME * result
				+ ((sentence == null) ? 0 : sentence.hashCode());
		result = PRIME * result
				+ ((variable == null) ? 0 : variable.hashCode());
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
		final Production other = (Production) obj;
		if (sentence == null) {
			if (other.sentence != null)
				return false;
		} else if (!sentence.equals(other.sentence))
			return false;
		if (variable == null) {
			if (other.variable != null)
				return false;
		} else if (!variable.equals(other.variable))
			return false;
		return true;
	}

	/**
	 * Obt�m representa��o em texto desta produ��o.
	 * 
	 * @return texto representando esta produ��o
	 */
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append(variable.toString() + " -> ");

		for (Symbol s : sentence) {
			builder.append(s.toString());
			builder.append(" ");
		}

		return builder.toString();
	}

	/**
	 * Cria uma c�pia desta produ��o.
	 * 
	 * @return c�pia desta produ��o
	 */
	public Object clone() {
		Production p = new Production();
		p.variable = variable;
		p.sentence = new Sentence(sentence);

		return p;
	}

	/**
	 * N�o � usado mais.
	 * 
	 * @param prod
	 * @return
	 */
	public boolean isLike(Production prod) {
		if (!prod.getVariable().equals(getVariable()))
			return false;

		Iterator<Symbol> prodIt = prod.getSentence().iterator();
		Iterator<Symbol> it = getSentence().iterator();

		for (/* */; prodIt.hasNext(); /* */) {
			if (!it.hasNext())
				return false;

			if (!prodIt.next().equals(it.next()))
				return false;
		}

		return true;
	}

	/**
	 * Determina se a produ��o � recursiva a esquerda.
	 * 
	 * @return true se a produ��o for recursiva a esquerda.
	 */
	public boolean isLeftRecursive() {
		return numSymbols() > 0 && getVariable().equals(getSentence().first());
	}
}