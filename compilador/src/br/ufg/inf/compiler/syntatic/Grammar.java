package br.ufg.inf.compiler.syntatic;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

public class Grammar {
    public NonTerminal startSymbol;

    public Set<Production> productions;

    private Grammar() {
	this(null);
    }

    public Grammar(NonTerminal startSymbol) {
	this.startSymbol = startSymbol;
	productions = new HashSet<Production>();
    }

    /**
         * Return the productions that are generated by a NonTerminal
         * 
         * @param var
         * @return
         */
    public Set<Production> getProductionsBySymbol(NonTerminal var) {
	Set<Production> result = new HashSet<Production>();

	for (Production p : productions) {
	    if (p.getVariable().equals(var)) {
		result.add(p);
	    }
	}

	return result;
    }

    /**
         * Creates a new NonTerminal associated with this grammar.
         * 
         * @return
         */
    public NonTerminal createNonTerminal() {
	return new NonTerminal();
    }

    /**
         * Create a clone of the grammar, useful for creating transformated
         * grammars.
         */
    public Object clone() {
	Grammar g = new Grammar();
	g.productions = new HashSet<Production>(productions);
	g.startSymbol = startSymbol;

	return g;
    }

    /**
         * Returns a new Grammar equivalent to this, but that has no Epsilon
         * rules, except for the start symbol.
         * 
         * @return
         */
    public Grammar getNoEpsilonGrammar() {
	Grammar g = (Grammar) clone();

	g.setNoEpsilon();

	return g;
    }

    /**
         * Return the set of Epsilon variables.
         * 
         * @return
         */
    private Set<NonTerminal> findEpsilonVariables() {
	Set<NonTerminal> result = new HashSet<NonTerminal>();
	int lastSize;

	for (Production p : productions) {
	    if (p.derivesEpsilon()) { /* derives Epsilon, so is an Epsilon-var */
		result.add(p.getVariable());
	    }
	}

	do {
	    lastSize = result.size();

	    for (Production p : productions) {
		/*
                 * if symbols in p is a subset of the Epsilon-vars. It's
                 * generation symbol is also an Epsilon-var
                 */
		if (p.symbolsIsSubsetOf(new HashSet<Symbol>(result))) {
		    result.add(p.getVariable());
		}
	    }
	} while (lastSize != result.size());

	return result;
    }

    /**
         * Removes recursion on the start symbol.
         * 
         */
    private void removeRecurStartSymbol() {
	for (Production p : getProductionsBySymbol(startSymbol)) {
	    if (p.isRecursive()) {
		Production startProduction = new Production(
			createNonTerminal(), new Symbol[] { startSymbol });
		startSymbol = startProduction.getVariable();
		productions.add(startProduction);
		break;
	    }
	}

    }

    /**
         * Remove Epsilon-vars from the grammar. Except from the start symbol,
         * of course.
         */
    private void setNoEpsilon() {
	/* Remove recursion on start symbol */
	removeRecurStartSymbol();

	/* find the Epsilon-vars */
	Set<NonTerminal> eVars = findEpsilonVariables();

	for (NonTerminal e : eVars) {
	    /* Remove productions that derive an Epsilon-var directly */
	    for (Iterator<Production> it = productions.iterator(); it.hasNext(); /* */) {
		Production p = it.next();
		if (p.getVariable().equals(e) && p.derivesEpsilon()) {
		    it.remove();
		}
	    }

	    /*
                 * Productions to be added, generated by removing the variable
                 * an Epsilon-var from the productions.
                 */
	    Set<Production> toAdd = new HashSet<Production>();

	    for (Production p : productions) {
		for (Production q : p.remove(e)) {
		    toAdd.add(q);
		}
	    }

	    productions.addAll(toAdd);
	}

	/*
         * If we have Epsilon-vars we have to add an Epsilon production to the
         * start symbol
         */
	if (eVars.size() > 0)
	    productions.add(new Production(startSymbol, new Symbol[] {}));
    }

    /**
         * String representation of the Grammar.
         * 
         * @return a string representation
         */
    public String toString() {
	StringBuilder builder = new StringBuilder();

	for (Production p : productions) {
	    builder.append(p.getVariable().equals(startSymbol) ? "-> " : "   ");
	    builder.append(p.toString() + "\n");
	}

	return builder.toString();
    }

    public static void main(String[] args) {
	NonTerminal A = new NonTerminal("A");
	NonTerminal B = new NonTerminal("B");
	NonTerminal C = new NonTerminal("C");
	NonTerminal S = new NonTerminal("S");

	Terminal a = new Terminal("a");
	Terminal b = new Terminal("b");
	Terminal c = new Terminal("c");

	Grammar g = new Grammar();
	g.startSymbol = S;

	g.productions.add(new Production(S, new Symbol[] { A, C, A }));

	g.productions.add(new Production(A, new Symbol[] { a, A, a }));
	g.productions.add(new Production(A, new Symbol[] { B }));
	g.productions.add(new Production(A, new Symbol[] { C }));

	g.productions.add(new Production(B, new Symbol[] { b, B }));
	g.productions.add(new Production(B, new Symbol[] { b }));

	g.productions.add(new Production(C, new Symbol[] { c, C }));
	g.productions.add(new Production(C, new Symbol[] {}));

	System.out.println(g);

	g.removeRecurStartSymbol();
	System.out.println(g);

	System.out.println("Epsilon");
	System.out.println(g.findEpsilonVariables());

	System.out.println("Removing C");
	g.setNoEpsilon();
	System.out.println(g.findEpsilonVariables());
	System.out.println(g);
    }
}
