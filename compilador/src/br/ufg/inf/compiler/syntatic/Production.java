package br.ufg.inf.compiler.syntatic;

import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

public class Production {
	private NonTerminal variable;

	private Sentence sentence;

	Production(NonTerminal variable, Symbol[] symbols) {
		this.variable = variable;
		this.sentence = new Sentence();
		Collections.addAll(this.sentence, symbols);
	}
	
	Production(NonTerminal variable, Sentence symbols) {
		this.variable = variable;
		this.sentence = new Sentence();
		this.sentence = symbols;
	}

	private Production() {
		variable = null;
		sentence = new Sentence();
	}

	public Sentence getSentence() {
		return sentence;
	}

	public NonTerminal getVariable() {
		return variable;
	}

	public boolean isRecursive() {
		return sentence.contains(variable);
	}

	public int numSymbols() {
		return getSentence().size();
	}

	public boolean derives(Symbol s) {
		return numSymbols() == 1 && getSentence().first().equals(s);
	}

	public boolean derivesEpsilon() {
		return numSymbols() == 0;
	}

	public boolean hasDerivation(Symbol s) {
		return sentence.contains(s);
	}

	/**
	 * Tests if the symbols of this production is a subset of a specified set.
	 * 
	 * @param s
	 *            set which will be tested if the symbol list is a subset
	 * @return true if this symbol list is a subset of s
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

	public boolean isCycle() {
		return numSymbols() == 1 && getSentence().first().equals(variable);
	}

	public boolean equals(Object o) {
		if (!(o instanceof Production))
			return false;

		Production p = (Production) o;

		return variable.equals(p.variable) && sentence.equals(p.sentence);
	}

	public int hashCode() {
		return variable.hashCode() * 1 + sentence.hashCode() * 3;
	}

	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append(variable.toString() + " -> ");

		for (Symbol s : sentence) {
			builder.append(s.toString());
			builder.append(" ");
		}

		return builder.toString();
	}

	public Object clone() {
		Production p = new Production();
		p.variable = variable;
		p.sentence = new Sentence(sentence);

		return p;
	}

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

	public boolean isLeftRecursive() {
		return numSymbols() > 0 && getVariable().equals(getSentence().first());
	}
}