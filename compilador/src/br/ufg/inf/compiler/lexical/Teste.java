package br.ufg.inf.compiler.lexical;

import java.io.IOException;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

import monq.jfa.CallbackException;
import monq.jfa.CharSequenceCharSource;
import monq.jfa.CharSource;
import monq.jfa.CompileDfaException;
import monq.jfa.Dfa;
import monq.jfa.DfaRun;
import monq.jfa.FaAction;
import monq.jfa.Nfa;
import monq.jfa.ReSyntaxException;

public class Teste {
	private Set<Token> symbolTable;

	private Nfa nfa;

	private List<Token> tokenList;
	
	private TokenManager manager;

	public Teste() {
		symbolTable = new HashSet<Token>();

		nfa = new Nfa(Nfa.NOTHING);

		tokenList = new LinkedList<Token>();
		
		manager = new TokenManager();
	}

	public void addTokenRule(final String tokenName, String tokenRule)
			throws ReSyntaxException {
		FaAction action = new FaAction() {

			public void invoke(StringBuffer arg0, int arg1, DfaRun arg2)
					throws CallbackException {
				Token token = manager.getToken(tokenName, arg0.toString());
				
				tokenList.add(token);
				symbolTable.add(token);
			}

			public FaAction mergeWith(FaAction arg0) {
				return null;
			}
		};

		nfa.or(tokenRule, action);
	}

	public void run(CharSource src) throws CompileDfaException, IOException {
		Dfa dfa = nfa.compile(DfaRun.UNMATCHED_THROW);

		DfaRun run = new DfaRun(dfa, src);

		run.filter();
	}

	public static void main(String[] args) throws CompileDfaException, ReSyntaxException, IOException {
		Teste t = new Teste();
		
		t.addTokenRule("ID", "[A-Za-z_][A-Za-z0-9_]*");
		t.addTokenRule("NUM", "[0-9]+");
		t.addTokenRule("SPACE", "[ \t\r\n]+");
		t.addTokenRule("OPEN_PAREN", "\\(");
		t.addTokenRule("CLOSE_PAREN", "\\)");
		
		t.run(new CharSequenceCharSource("if (true) 10"));
		
		for (Token tok : t.tokenList) {
			System.out.println(tok);
		}	
	}
}
