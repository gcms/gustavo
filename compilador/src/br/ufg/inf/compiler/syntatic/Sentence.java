package br.ufg.inf.compiler.syntatic;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import br.ufg.inf.compiler.main.SetupException;

/**
 * Representa uma sequencia de símbolos, terminais ou não terminais. Representa
 * uma forma sentencial.
 * 
 * @author gustavo
 * 
 */
public class Sentence implements Iterable<Symbol>, Collection<Symbol> {

	/** Lista de símbolos da seqüencia. */
	private List<Symbol> symbols;

	/**
	 * Cria uma nova forma sentencial.
	 * 
	 * @param sentence
	 */
	public Sentence(Sentence sentence) {
		this.symbols = new ArrayList<Symbol>(symbols);
	}

	/**
	 * Cria uma nova sentença a partir de uma string. É seguida uma convenção
	 * onde cada símbolo tem apenas um caractere, sendo caracteres maiúsculas
	 * não terminais, e minúsulas terminais.
	 * 
	 * @param sentence
	 */
	public Sentence(String sentence) {
		symbols = new ArrayList<Symbol>();

		for (int i = 0; i < sentence.length(); i++) {
			char ch = sentence.charAt(i);
			if (Character.isUpperCase(ch)) {
				symbols.add(new NonTerminal(Character.toString(ch)));
			} else if (Character.isLowerCase(ch)) {
				symbols.add(new Terminal(Character.toString(ch)));
			} else {
				throw new SetupException("Sentence: Caractere inválido");
			}
		}
	}

	public Sentence(Collection<? extends Symbol> symbols) {
		this.symbols = new ArrayList<Symbol>(symbols);
	}

	public Sentence() {
		this.symbols = new ArrayList<Symbol>();
	}

	public Symbol first() {
		return symbols.get(0);
	}

	public Iterator<Symbol> iterator() {
		return symbols.iterator();
	}

	public boolean add(Symbol e) {
		return symbols.add(e);
	}

	public boolean addAll(Collection<? extends Symbol> c) {
		return symbols.addAll(c);
	}

	public void clear() {
		symbols.clear();
	}

	public boolean contains(Object o) {
		return symbols.contains(o);
	}

	public boolean containsAll(Collection<?> c) {
		return symbols.containsAll(c);
	}

	public boolean isEmpty() {
		return symbols.isEmpty();
	}

	public boolean remove(Object o) {
		return symbols.remove(o);
	}

	public boolean removeAll(Collection<?> c) {
		return symbols.removeAll(c);
	}

	public boolean retainAll(Collection<?> c) {
		return symbols.retainAll(c);
	}

	public int size() {
		return symbols.size();
	}

	public Object[] toArray() {
		return symbols.toArray();
	}

	public <T> T[] toArray(T[] a) {
		return symbols.toArray(a);
	}

	/**
	 * Cria uma subsequencia contendo os elementos entre os indices i e j.
	 * 
	 * @param i
	 * @param j
	 * @return nova sequencia
	 */
	public Collection<? extends Symbol> subSentence(int i, int j) {
		return new Sentence(symbols.subList(i, j));
	}

	/**
	 * Obtém uma subsequencia desta sequencia, que inicia no símbolo
	 * imediatamente após a primeira ocorrência de s nesta sequencia.
	 * 
	 * @param s
	 *            símbolo
	 * @return nova sequencia, ou null se s não estiver nesta sequencia
	 */
	public Sentence subSentenceAfter(Symbol s) {
		for (int i = 0; i < size(); i++) {
			if (symbols.get(i).equals(s)) {
				return new Sentence(subSentence(i + 1, symbols.size()));
			}
		}

		return null;
	}

	/**
	 * Remove o primeiro símbolo da seqüencia.
	 * 
	 */
	public void removeFirst() {
		symbols.remove(0);
	}

	/**
	 * Representação em texto.
	 */
	public String toString() {
		return symbols.toString();
	}

	/**
	 * Obtém um iterator que varre na ordem reversa a sequencia.
	 * 
	 * @return iterator
	 */
	public Iterator<Symbol> reverseIterator() {
		return new Iterator<Symbol>() {

			private int pos = symbols.size();

			public boolean hasNext() {
				return pos > 0;
			}

			public Symbol next() {
				pos--;
				return symbols.get(pos);
			}

			public void remove() {
				throw new UnsupportedOperationException();
			}

		};
	}

	@Override
	public int hashCode() {
		final int PRIME = 31;
		int result = 1;
		result = PRIME * result + ((symbols == null) ? 0 : symbols.hashCode());
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
		final Sentence other = (Sentence) obj;
		if (symbols == null) {
			if (other.symbols != null)
				return false;
		} else if (!symbols.equals(other.symbols))
			return false;
		return true;
	}

	/**
	 * Teste.
	 * 
	 * @param args
	 */
	public static void main(String[] args) {
		Sentence s = new Sentence("asdf");

		for (Iterator it = s.reverseIterator(); it.hasNext();) {
			System.out.print(it.next());
		}
		System.out.println();
	}
}
