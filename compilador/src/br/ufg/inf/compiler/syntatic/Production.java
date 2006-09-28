package br.ufg.inf.compiler.syntatic;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

public class Production {
    private NonTerminal variable;

    private List<Symbol> symbols;

    public Production(NonTerminal variable, Symbol[] symbols) {
	this.variable = variable;
	this.symbols = new ArrayList<Symbol>();
	Collections.addAll(this.symbols, symbols);
    }

    private Production() {
	variable = null;
	symbols = new ArrayList<Symbol>();
    }

    public List<Symbol> getSymbols() {
	return symbols;
    }

    public NonTerminal getVariable() {
	return variable;
    }

    public boolean isRecursive() {
	return symbols.contains(variable);
    }

    public int numSymbols() {
	return getSymbols().size();
    }

    public boolean derives(Symbol s) {
	return numSymbols() == 1 && getSymbols().get(0).equals(s);
    }

    public boolean derivesEpsilon() {
	return numSymbols() == 0;
    }

    public boolean hasDerivation(Symbol s) {
	return symbols.contains(s);
    }

    /**
         * Tests if the symbols of this production is a subset of a specified
         * set.
         * 
         * @param s
         *                set which will be tested if the symbol list is a
         *                subset
         * @return true if this symbol list is a subset of s
         */
    public boolean symbolsIsSubsetOf(Set<Symbol> s) {
	for (Symbol e : symbols) {
	    if (!s.contains(e))
		return false;
	}

	return true;
    }

    /**
         * Generates all arrangements of symbols removing or not the symbol e
         * from the symbol list. Its intended to be used in the process of
         * eliminating Epsilon variables as this behaviour is the same as
         * exchanging the symbol for Epsilon.
         * 
         * @param e
         *                symbol to be substitued for Epsilon
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
		for (Iterator<Symbol> it = clone.symbols.iterator(); it
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

    public boolean equals(Object o) {
	if (!(o instanceof Production))
	    return false;

	Production p = (Production) o;

	return variable.equals(p.variable) && symbols.equals(p.symbols);
    }

    public int hashCode() {
	return variable.hashCode() * 1 + symbols.hashCode() * 3;
    }

    public String toString() {
	StringBuilder builder = new StringBuilder();
	builder.append(variable.toString() + " -> ");

	for (Symbol s : symbols) {
	    builder.append(s.toString());
	    builder.append(" ");
	}

	return builder.toString();
    }

    public Object clone() {
	Production p = new Production();
	p.variable = variable;
	p.symbols = new ArrayList<Symbol>(symbols);

	return p;
    }
}
