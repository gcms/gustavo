package br.ufg.inf.compiler.syntatic;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.Stack;

import br.ufg.inf.compiler.lexical.Lexer;
import br.ufg.inf.compiler.lexical.Lexeme;
import br.ufg.inf.compiler.lexical.Token;
import br.ufg.inf.compiler.main.SetupException;

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
			throw new SetupException(
					"SyntaticTable: Duas producoes na mesma celula\n" + "Em ["
							+ nonTerm + ", " + term + "]: "
							+ getEntry(nonTerm, term) + " " + production);

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
				Set<Terminal> first = first(s);

				result.addAll(first);
				result.remove(Terminal.EPSILON);

				if (first.contains(Terminal.EPSILON) || s.size() == 0) {
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

	private class LexerTokenStream implements TokenStream {
		private Lexer lexer;

		private Lexeme current;

		public LexerTokenStream(Lexer lexer) {
			this.lexer = lexer;
		}

		public Lexeme getToken() {
			if (current != null)
				return current;

			advance();
			return current;
		}

		public void advance() {
			Lexeme token = lexer.nextToken();

			current = token;
		}
	}

	public boolean parse(Lexer lexer) {
		return parse(new LexerTokenStream(lexer));
	}

	private class SentenceTokenStream implements TokenStream {
		private Iterator<Symbol> it;

		private Terminal data;

		public SentenceTokenStream(Sentence sentence) {
			it = sentence.iterator();
		}

		public void advance() {
			if (it.hasNext())
				data = (Terminal) it.next();
			else
				data = Terminal.END;
		}

		public Lexeme getToken() {
			if (data == null)
				advance();

			return new Lexeme(data.getName(), new Token(data.getName(), null));
		}
	}

	public boolean parse(Sentence sentence) {
		return parse(new SentenceTokenStream(sentence));
	}

	public boolean parse(TokenStream stream) {
		Stack<Symbol> stack = new Stack<Symbol>();

		stack.push(Terminal.END);
		stack.push(grammar.getStartSymbol());

		Symbol x;
		Terminal a;
		Lexeme t;
		do {
			x = stack.peek();
			t = stream.getToken();
			a = (t == null ? Terminal.END : new Terminal(t.getTokenName()));

			if (x instanceof Terminal) {
				if (x.equals(a)) {
					stack.pop();
					stream.advance();
				} else {
					return false;
				}
			} else {
				assert (x instanceof NonTerminal);
				Production p = getEntry((NonTerminal) x, a);
				System.out.println("getEntry(" + x + ", " + a + ") = " + p);
				if (p != null && p.getVariable().equals(x)) {
					stack.pop();

					for (Iterator<Symbol> it = p.getSentence()
							.reverseIterator(); it.hasNext();) {
						stack.push(it.next());
					}

					System.out.println(p);
				} else {
					return false;
				}

			}
		} while (!x.equals(Terminal.END));

		return true;
	}

	public static void main(String[] args) {
		Grammar g = new Grammar(new NonTerminal("E"));

		g.addProduction('E', "TbAc");
		g.addProduction('T', "i");
		g.addProduction('A', "a");
		g.addProduction('T', "");

		SyntaticTable t = new SyntaticTable(g);

		System.out.println(t.parse(new Sentence("bac")));
	}
}
