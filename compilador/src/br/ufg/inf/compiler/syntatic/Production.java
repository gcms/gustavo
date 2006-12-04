package br.ufg.inf.compiler.syntatic;

import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 * Representa uma produção em uma gramática.
 * 
 * @author gustavo
 * 
 */
public class Production {
	/** Lado esquerdo da produção. */
	private NonTerminal variable;

	/** Lado direito da produção. */
	private Sentence sentence;

	/**
	 * Cria uma produção
	 * 
	 * @param variable
	 *            variável (lado esquerdo)
	 * @param symbols
	 *            símbolos (lado direito)
	 */
	Production(NonTerminal variable, Symbol[] symbols) {
		this.variable = variable;
		this.sentence = new Sentence();
		Collections.addAll(this.sentence, symbols);
	}

	/**
	 * Cria uma produção
	 * 
	 * @param variable
	 *            variável (lado esquerdo)
	 * @param symbols
	 *            símbolos (lado direito)
	 */
	Production(NonTerminal variable, Sentence symbols) {
		this.variable = variable;
		this.sentence = new Sentence();
		this.sentence = symbols;
	}

	/**
	 * Cria uma produção.
	 */
	private Production() {
		variable = null;
		sentence = new Sentence();
	}

	/**
	 * Obtém o lado direito da produção.
	 * 
	 * @return forma sentencial (lado direito)
	 */
	public Sentence getSentence() {
		return sentence;
	}

	/**
	 * Obtém o lado esquerdo da produção
	 * 
	 * @return variável (lado esquerdo)
	 */
	public NonTerminal getVariable() {
		return variable;
	}

	/**
	 * Determina se a produção é recursiva.
	 * 
	 * @return true se a produção é recursiva
	 */
	public boolean isRecursive() {
		return sentence.contains(variable);
	}

	/**
	 * Retorna o tamanho do lado direito da produção
	 * 
	 * @return número de símbolos do lado direito da produção
	 */
	public int numSymbols() {
		return getSentence().size();
	}

	/**
	 * Determina se a produção deriva o símbolo s.
	 * 
	 * @param sa
	 *            true se esta produção deriva s
	 * @return
	 */
	public boolean derives(Symbol s) {
		return numSymbols() == 1 && getSentence().first().equals(s);
	}

	/**
	 * Determina se a produção deriva EPSILON.
	 * 
	 * @return true se esta produção deriva EPSILON
	 */
	public boolean derivesEpsilon() {
		return numSymbols() == 0;
	}

	/**
	 * Determina se o lado direito da produção contém o símbolo s.
	 * 
	 * @param s
	 *            símbolo a ser procurado
	 * @return se s está no lado direito da produção
	 */
	public boolean hasDerivation(Symbol s) {
		return sentence.contains(s);
	}

	/**
	 * Testa se os símbolos desta produção são um subconjunto do conjunto
	 * especificado.
	 * 
	 * @param s
	 *            conjunto de símbolos especificado
	 * @return true se o lado direito é subconjunto de s
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
	 * Determina se a produção cicla.
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
	 * Obtém representação em texto desta produção.
	 * 
	 * @return texto representando esta produção
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
	 * Cria uma cópia desta produção.
	 * 
	 * @return cópia desta produção
	 */
	public Object clone() {
		Production p = new Production();
		p.variable = variable;
		p.sentence = new Sentence(sentence);

		return p;
	}

	/**
	 * Não é usado mais.
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
	 * Determina se a produção é recursiva a esquerda.
	 * 
	 * @return true se a produção for recursiva a esquerda.
	 */
	public boolean isLeftRecursive() {
		return numSymbols() > 0 && getVariable().equals(getSentence().first());
	}
}