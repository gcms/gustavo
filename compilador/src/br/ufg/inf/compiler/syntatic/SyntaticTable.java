package br.ufg.inf.compiler.syntatic;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class SyntaticTable {

	private static class Entry {
		private NonTerminal nonTerm;

		private Terminal term;

		public int hashCode() {
			return nonTerm.hashCode() + 2 * term.hashCode();
		}

		public boolean equals(Object o) {
			if (!(o instanceof Entry))
				return false;

			Entry e = (Entry) o;

			return term.equals(e.term) && nonTerm.equals(e.nonTerm);

		}

		public Entry(NonTerminal nonTerm, Terminal term) {
			this.nonTerm = nonTerm;
			this.term = term;
		}

		public String toString() {
			return "[" + nonTerm + ", " + term + "]";
		}
	}

	/**
	 * The table itself, stores entries (a terminal/non-terminal pair) as the
	 * key and productions as values.
	 */
	private Map<Entry, Production> table;

	/**
	 * Returns the production to be used when nonTerm is on the stack and term
	 * is on the stream.
	 * 
	 * @param nonTerm
	 *            NonTerminal position
	 * @param term
	 *            Terminal position
	 * @return Production to be used
	 */
	private Production getEntry(NonTerminal nonTerm, Terminal term) {
		return table.get(new Entry(nonTerm, term));
	}

	private void setEntry(NonTerminal nonTerm, Terminal term,
			Production production) {
		if (getEntry(nonTerm, term) != null)
			throw new RuntimeException("duas producoes na mesma celula");

		table.put(new Entry(nonTerm, term), production);
	}

	private Grammar grammar;

	public SyntaticTable(Grammar grammar) {
		this.grammar = grammar;
		table = new HashMap<Entry, Production>();

		fillTable();
	}

	private Set<Terminal> first(Sentence sentence) {
		Set<Terminal> result = new HashSet<Terminal>();

		if (sentence.size() == 0) {
			result.add(Terminal.EPSILON);
			return result;
		}

		if (sentence.first() instanceof Terminal) {
			result.add((Terminal) sentence.first());

			return result;
		}

		assert (sentence.first() instanceof NonTerminal);

		for (Symbol s : sentence) {
			/*
			 * Remove EPSILON from result, so that we result doesn't returns
			 * EPSILON in the set when first doesn't contains EPSILON.
			 */
			result.remove(Terminal.EPSILON);

			if (s instanceof Terminal) {
				result.add((Terminal) s);
				break;
			}

			NonTerminal n = (NonTerminal) s;

			Set<Terminal> first = new HashSet<Terminal>();
			for (Production p : grammar.getProductionsBySymbol(n)) {
				first.addAll(first(p.getSentence()));
			}

			/*
			 * If first doesn't contain EPSILON add all the terminals to the
			 * result and break to return it
			 */
			if (!first.contains(Terminal.EPSILON)) {
				result.addAll(first);
				break;
			}

			result.addAll(first);
		}

		return result;
	}

	private Set<Terminal> follows(NonTerminal nonTerm) {
		Set<Terminal> result = new HashSet<Terminal>();

		/* Add the $ symbol to FOLLOW(StartSymbol) */
		if (grammar.getStartSymbol().equals(nonTerm)) {
			result.add(Terminal.END);
		}

		/*
		 * Finds the productions where the right hand side is like wAv, where A
		 * is the nonTerm used as argument to this function and w and v are
		 * other sentences. Then add first(v) to the result. If first(v)
		 * contains EPSILON, and v is a sentence in the form Ay add first(y) and
		 * so on. This algorithm is described on dragon book "Compilers:
		 * Principles, Tools and Techniques" Chapter 4
		 */
		for (Production p : grammar.getProductions()) {
			Sentence s = p.getSentence().subSentenceAfter(nonTerm);

			if (s != null) {
				for (Set<Terminal> first = first(s); s.size() > 0; first = first(s)) {
					result.addAll(first);
					result.remove(Terminal.EPSILON);

					System.out.println(s.size());
					s.removeFirst();

					if (!first.contains(Terminal.EPSILON))
						break;
				}

				if (s.size() == 0) {
					result.addAll(follows(p.getVariable()));
				}

			}
		}

		return result;
	}

	private void fillTable() {

		for (Production p : grammar.getProductions()) {
			Set<Terminal> first = first(p.getSentence());
			for (Terminal t : first) {
				if (!t.equals(Terminal.EPSILON))
					setEntry(p.getVariable(), t, p);
			}

			if (first.contains(Terminal.EPSILON)) {
				for (Terminal t : follows(p.getVariable())) {
					if (!t.equals(Terminal.EPSILON))
						setEntry(p.getVariable(), t, p);
				}
			}
		}
	}

	public static void main(String[] args) {
		Grammar g = new Grammar(new NonTerminal("E"));

		g.addProduction('E', "Tbc");
		g.addProduction('T', "i");
		g.addProduction('A', "a");
		g.addProduction('T', "");

		SyntaticTable t = new SyntaticTable(g);

		System.out.println(t.first(new Sentence("T")));
		System.out.println(t.follows(new NonTerminal("E")));

		System.out.println(t.table);
	}
}
