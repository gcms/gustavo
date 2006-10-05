package br.ufg.inf.compiler.lexical;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;

import monq.jfa.ByteCharSource;
import monq.jfa.CallbackException;
import monq.jfa.CharSource;
import monq.jfa.CompileDfaException;
import monq.jfa.Dfa;
import monq.jfa.DfaRun;
import monq.jfa.FaAction;
import monq.jfa.Nfa;
import monq.jfa.ReSyntaxException;

public class Lexer {
	private Set<Token> symbolTable;

	private Nfa nfa;

	private Queue<Token> tokenList;

	private TokenManager manager;

	private boolean started;

	private boolean finished;

	private Thread t;

	public Lexer(final CharSource src) {
		symbolTable = new HashSet<Token>();

		nfa = new Nfa(Nfa.NOTHING);

		tokenList = new LinkedList<Token>();

		manager = new TokenManager();

		t = new Thread() {
			public void run() {
				try {
					Lexer.this.run(src);
				} catch (CompileDfaException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				synchronized (Lexer.this) {
					finished = true;
					Lexer.this.notifyAll();
				}
			}
		};
	}

	public void addTokenRule(final String tokenName, String tokenRule)
			throws ReSyntaxException {
		FaAction action = new FaAction() {

			public void invoke(StringBuffer arg0, int arg1, DfaRun arg2)
					throws CallbackException {
				Token token = manager.getToken(tokenName, arg0.toString());

				tokenList.offer(token);
				symbolTable.add(token);

				synchronized (Lexer.this) {
					Lexer.this.notifyAll();
				}
			}

			public FaAction mergeWith(FaAction arg0) {
				return null;
			}
		};

		nfa.or(tokenRule, action);
	}

	private void run(CharSource src) throws CompileDfaException, IOException {
		Dfa dfa = nfa.compile(DfaRun.UNMATCHED_THROW);

		DfaRun run = new DfaRun(dfa, src);

		run.filter();
	}

	public Token getToken() {
		synchronized (this) {
			if (!started) {
				started = true;
				t.start();
			}

			while (!finished && tokenList.isEmpty()) {
				try {
					wait();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			return tokenList.poll();
		}
	}

	public static void main(String[] args) throws CompileDfaException,
			ReSyntaxException, IOException {
		Lexer t = new Lexer(
				new ByteCharSource(new FileInputStream("teste.txt")));

		t.addTokenRule("ID", "[A-Za-z_][A-Za-z0-9_]*");
		t.addTokenRule("NUM", "[0-9]+");
		t.addTokenRule("SPACE", "[ \t\r\n]+");
		t.addTokenRule("OPEN_PAREN", "\\(");
		t.addTokenRule("CLOSE_PAREN", "\\)");

		Token tok;

		while ((tok = t.getToken()) != null) {
			System.out.println(tok);
		}

	}
}
