package br.ufg.inf.compiler.syntatic;

import java.util.ArrayList;
import java.util.Collections;
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
	}

	private Map<Entry, Production> table;

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

	private void fillTable() {
		for (Production p : grammar.getProductions()) {
		}
	}

	public static void main(String[] args) {
		Grammar g = new Grammar(new NonTerminal("E"));

		g.addProduction('E', "Tb");
		g.addProduction('T', "i");
		g.addProduction('A', "a");
		g.addProduction('T', "");

		SyntaticTable t = new SyntaticTable(g);

		System.out.println(t.first(new Sentence("T")));
	}
}
