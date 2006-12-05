package br.ufg.inf.compiler.lexical;

import java.io.IOException;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;

import br.ufg.inf.compiler.main.LexicalException;

import monq.jfa.AbstractFaAction;
import monq.jfa.CallbackException;
import monq.jfa.CharSource;
import monq.jfa.CompileDfaException;
import monq.jfa.Dfa;
import monq.jfa.DfaRun;
import monq.jfa.FaAction;
import monq.jfa.Nfa;
import monq.jfa.NomatchException;
import monq.jfa.ReSyntaxException;

/**
 * Classe utilizada para representar um analisador lexico. Esta classe lexico
 * pode ser usado para gerar uma sequencia de tokens, a partir de uma sequencia
 * de caracteres.
 * 
 * @author gustavo
 * 
 */
public class Lexer {
	/** Tabela de simbolos. */
	private Set<Lexeme> symbolTable;

	/** Nfa representando o processamento da entrada de caracteres. */
	private Nfa nfa;

	/** Fila de tokens processados. */
	private Queue<Lexeme> tokenList;

	/**
	 * Gerenciador de tokens, usado para tornar o mapeamento token -> tipo, mais
	 * agil.
	 */
	private LexerSpec manager;

	/** Utilizado para o controle da thread onde o automato eh executado. */
	private boolean started;

	/** Utilizado para o controle da thread onde o automato eh executado. */
	private boolean finished;

	/** Thread onde o processamento e realizado. */
	private Thread t;

	private int priority = Integer.MAX_VALUE;

	/**
	 * Cria um novo analisador lexico, a partir de uma fonte de caracteres.
	 * 
	 * @param src
	 *            objeto responsavel por gerar um fluxo de caracteres
	 */
	public Lexer(LexerSpec spec, final CharSource src) {
		symbolTable = new HashSet<Lexeme>();

		nfa = new Nfa(Nfa.NOTHING);

		tokenList = new LinkedList<Lexeme>();

		manager = spec;

		/*
		 * O automato responsavel pelo processamento, eh executado nesta nova
		 * thread. Quando um novo token eh identificado, ele eh adicionado a
		 * lista de tokens, e a thread gera uma notificacao, para que caso
		 * alguma outra thread esteja tentando obter o proximo token possa
		 * desbloquear.
		 */
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

				/* thread finalizou o processamento, finished = true */
				synchronized (Lexer.this) {
					finished = true;
					Lexer.this.notifyAll();
				}
			}
		};
	}

	/**
	 * Adiciona uma nova regra a expecificacao lexica do analisador. A
	 * especificacao lexica, consiste em uma serie de pares do tipo <Expressao
	 * Regular, Nome do Token>. Esta especificacao eh utilizada pelo analisador
	 * para identificar qual token serah gerado, de acordo com os valores na
	 * entrada de caracteres.
	 * 
	 * @param tokenName
	 *            nome do token
	 * @param tokenRule
	 *            expressao regular indicando regra de formacao do token
	 * @throws ReSyntaxException
	 *             no caso de um erro na formacao da expressao regular
	 */
	void addTokenRule(final String tokenName, String tokenRule)
			throws ReSyntaxException {
		class MyAction extends AbstractFaAction {
			private static final long serialVersionUID = -6948125824009493478L;

			public MyAction() {
				setPriority(Lexer.this.priority--);
			}

			public void invoke(StringBuffer arg0, int arg1, DfaRun arg2)
					throws CallbackException {
				Lexeme token = manager.getToken(tokenName, arg0.toString());

				/* adiciona o token a fila de tokens e a tabela de simbolos */
				tokenList.offer(token);
				symbolTable.add(token);

				/* notifica threads esperando */
				synchronized (Lexer.this) {
					Lexer.this.notifyAll();
				}
			}
		}
		;

		FaAction action;
		if (tokenName == null || tokenName.equals("")) {
			action = new MyAction() {
				private static final long serialVersionUID = -7066473078024093880L;

				public void invoke(StringBuffer arg0, int arg1, DfaRun arg2)
						throws CallbackException {
				}
			};
		} else {
			action = new MyAction();
		}

		nfa.or(tokenRule, action);
	}

	/**
	 * Executa o processamento da cadeia de entrada, gerando uma sequencia de
	 * tokens que é armazenada em um fila.
	 * 
	 * @param src
	 *            fonte de caracteres
	 * @throws CompileDfaException
	 * @throws IOException
	 */
	private void run(CharSource src) throws CompileDfaException, IOException {
		Dfa dfa = nfa.compile(DfaRun.UNMATCHED_THROW, new FaAction() {

			public void invoke(StringBuffer arg0, int arg1, DfaRun arg2)
					throws CallbackException {
				// TODO Auto-generated method stub

			}

			public FaAction mergeWith(FaAction arg0) {
				// TODO Auto-generated method stub
				return null;
			}

		});

		DfaRun run = new DfaRun(dfa, src);

		try {
			run.filter();
		} catch (NomatchException e) {
			throw new LexicalException("Lexer: Erro lexico\n"
					+ e.getLocalizedMessage());
		}
	}

	/**
	 * Obtém o próximo token. Consome um token da fila de tokens. Se a fila está
	 * vazia, espera a thread produtora criar um novo token.
	 * 
	 * @return próximo token
	 */
	public Lexeme nextLexeme() {
		synchronized (this) {
			/* inicia a thread the processamento (caso nao tenha iniciado) */
			if (!started) {
				started = true;
				t.start();
			}

			/* espera notificacao, de token na fila */
			while (!finished && tokenList.isEmpty()) {
				try {
					wait();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			/* retorna token no inicio da fila, e o remove dela */
			return tokenList.poll();
		}
	}
	/*
	 * public static void main(String[] args) throws CompileDfaException,
	 * ReSyntaxException, IOException { Lexer t = new Lexer( new
	 * ByteCharSource(new FileInputStream("teste.txt")));
	 * 
	 * t.addTokenRule("ID", "[A-Za-z_][A-Za-z0-9_]*"); t.addTokenRule("NUM",
	 * "[0-9]+"); t.addTokenRule("SPACE", "[ \t\r\n]+");
	 * t.addTokenRule("OPEN_PAREN", "\\("); t.addTokenRule("CLOSE_PAREN",
	 * "\\)");
	 * 
	 * Token tok;
	 * 
	 * while ((tok = t.nextToken()) != null) { System.out.println(tok); } }
	 */
}
