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

		// System.out.println("installing [" + nonTerm + ", " + term + "] = "
		// + production);
		table.put(new Entry(nonTerm, term), production);
	}

	private Grammar grammar;

	public SyntaticTable(Grammar grammar) {
		System.out.println("Criando tabela sintática...");
		this.grammar = grammar;
		table = new HashMap<Entry, Production>();

		fillTable();
	}

	private Set<Terminal> first(Sentence sentence) {
		Set<Terminal> result = new HashSet<Terminal>();
		// System.out.println("+first(" + sentence + ")");

		if (sentence.size() == 0) {
			result.add(Terminal.EPSILON);
		} else if (sentence.first() instanceof Terminal) {
			result.add((Terminal) sentence.first());
		} else {
			Set<NonTerminal> visited = new HashSet<NonTerminal>();
			for (Symbol symb : sentence) {
				if (visited.contains(symb))
					continue;
				if (symb instanceof NonTerminal) {
					visited.add((NonTerminal) symb);
				}

				result.remove(Terminal.EPSILON);
				for (Production p : grammar
						.getProductionsBySymbol((NonTerminal) symb)) {
					result.addAll(first(p.getSentence()));
				}

				if (!result.contains(Terminal.EPSILON)) {
					break;
				}
			}

		}

		// System.out.println("-first(" + sentence + ") = " + result);
		return result;
		// assert (sentence.first() instanceof NonTerminal);
		//
		// for (Symbol s : sentence) {
		// /*
		// * Remove EPSILON from result, so that we result doesn't returns
		// * EPSILON in the set when first doesn't contains EPSILON.
		// */
		// result.remove(Terminal.EPSILON);
		//
		// if (s instanceof Terminal) {
		// result.add((Terminal) s);
		// break;
		// }
		//
		// NonTerminal n = (NonTerminal) s;
		//
		// Set<Terminal> first = new HashSet<Terminal>();
		// for (Production p : grammar.getProductionsBySymbol(n)) {
		// System.out.println(p);
		//
		// first.addAll(first(p.getSentence()));
		// }
		//
		// /*
		// * If first doesn't contain EPSILON add all the terminals to the
		// * result and break to return it
		// */
		// if (!first.contains(Terminal.EPSILON)) {
		// result.addAll(first);
		// break;
		// }
		//
		// result.addAll(first);
		// }
		//
		// System.out.println("-first(" + sentence + ")");
		// return result;
	}

	private Set<NonTerminal> following = new HashSet<NonTerminal>();

	private Map<NonTerminal, Set<Terminal>> followings = new HashMap<NonTerminal, Set<Terminal>>();

	private Set<Terminal> follows(NonTerminal nonTerm) {
		Set<Terminal> result = new HashSet<Terminal>();

		System.out.println("+follows(" + nonTerm + ")");

		if (followings.get(nonTerm) != null) {
			following.remove(nonTerm);
			System.out.println("-follows(" + nonTerm + ")");
			return followings.get(nonTerm);
		}

		following.add(nonTerm);

		// System.out.println("-follows(" + nonTerm + ")");

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
					if (!p.getVariable().equals(nonTerm)
							&& !following.contains(p.getVariable()))
						result.addAll(follows(p.getVariable()));
				}
			}
		}

		followings.put(nonTerm, result);

		following.remove(nonTerm);
		System.out.println("-follows(" + nonTerm + ") = " + result);
		return result;
	}

	public Map<NonTerminal, Set<Terminal>> following() {
		Map<NonTerminal, Set<Terminal>> following = new HashMap<NonTerminal, Set<Terminal>>();
		Map<NonTerminal, Set<Terminal>> aux = new HashMap<NonTerminal, Set<Terminal>>();

		do {
			aux.putAll(following);
			for (NonTerminal v : grammar.getNonTerminals()) {
				Set<Terminal> s = following.get(v);
				if (s == null) {
					s = new HashSet<Terminal>();
				} else {
					Set<Terminal> tmp = s;
					s = new HashSet<Terminal>();
					s.addAll(tmp);
				}

				if (grammar.getStartSymbol().equals(v)) {
					s.add(Terminal.END);
				}

				Set<Production> rules = grammar.getProductionsWith(v);

				for (Production r : rules) {
					Sentence p = r.getSentence();

					int start = 0;

					do {
						start = p.get(v, start);

						if (start < 0)
							continue;
						start++;

						Sentence beta = p.subSentence(start, p.size());

						Set<Terminal> temp = first(beta);
						s.addAll(temp);
						if (temp.contains(Terminal.EPSILON)) {
							s.remove(Terminal.EPSILON);
							NonTerminal a = r.getVariable();
							if (following.get(a) != null) {
								s.addAll(following.get(a));
							}
						}

					} while (start >= 0);
				}
				following.put(v, s);
			}
		} while (!aux.equals(following));

		return following;
	}

	private void fillTable() {
		Map<NonTerminal, Set<Terminal>> followings = following();

		System.out.println("Preenchendo entradas na tabela...");
		for (Production p : grammar.getProductions()) {
			// System.out.println("fillTable to " + p);
			Set<Terminal> first = first(p.getSentence());
			for (Terminal t : first) {
				if (!t.equals(Terminal.EPSILON))
					setEntry(p.getVariable(), t, p);
			}

			if (first.contains(Terminal.EPSILON)) {
				for (Terminal t : followings.get(p.getVariable())) {
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
			Lexeme token = lexer.nextLexeme();

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

		System.out.println("Processando entrada...");

		stack.push(Terminal.END);
		stack.push(grammar.getStartSymbol());

		Symbol x;
		Terminal a;
		Lexeme t;

		do {
			x = stack.peek();
			t = stream.getToken();
			a = (t == null ? Terminal.END : new Terminal(t.getTokenName()));

			System.out.println("PILHA = " + x + ",\tENTRADA = " + t);

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

				if (p != null && p.getVariable().equals(x)) {
					stack.pop();

					for (Iterator<Symbol> it = p.getSentence()
							.reverseIterator(); it.hasNext();) {
						stack.push(it.next());
					}

					System.out.println("\tTABELA[" + x + ", " + a + "] = ( "
							+ p + " )\n");
				} else {
					return false;
				}

			}
		} while (!x.equals(Terminal.END));

		return true;
	}

	public String toString() {
		StringBuilder builder = new StringBuilder();

		for (Map.Entry<Entry, Production> e : table.entrySet()) {
			Entry entry = e.getKey();
			Production prod = e.getValue();
			builder.append("[" + entry.nonTerm + ", " + entry.term + "] = "
					+ prod + "\n");
		}

		return builder.toString();
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
