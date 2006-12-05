package br.ufg.inf.compiler.lexical;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import br.ufg.inf.compiler.main.SetupException;

import monq.jfa.CharSequenceCharSource;
import monq.jfa.CharSource;
import monq.jfa.Nfa;
import monq.jfa.ReSyntaxException;

/**
 * Classe utilizada para representar uma especificação léxico. Esta classe é
 * usada posteriormente para gerar um analisador léxico, capaz de examinar uma
 * sequência de caracteres e convertê-la para uma sequência de tokens.
 * 
 * @author gustavo
 * 
 */
public class LexerSpec {

	/** Mapa de especificação de tokens. */
	private Map<String, Token> tokenSpec;

	/** Lista ordenada de especificação. */
	private List<Token> tokens;

	/** Nfa usado para testar a validade da expressão regular. */
	private Nfa nfa;

	/**
	 * Cria uma nova especificação léxica.
	 */
	public LexerSpec() {
		tokenSpec = new HashMap<String, Token>();
		tokens = new LinkedList<Token>();

		nfa = new Nfa(Nfa.NOTHING);
	}

	/**
	 * Adiciona uma regra léxica a especificação.
	 * 
	 * @param tokenName
	 *            nome da regra léxica
	 * @param tokenRule
	 *            expressão regular da regra
	 * @throws ReSyntaxException
	 */
	public void addTokenRule(final String tokenName, String tokenRule)
			throws ReSyntaxException {
		Token token = new Token(tokenName, tokenRule);

		tokenSpec.put(tokenName, token);
		tokens.add(token);

		nfa.or(tokenRule);
	}

	/**
	 * Cria um analisador léxico para a fonte de caracteres especificada.
	 * 
	 * @param src
	 *            fonte de caracteres
	 * @return um novo analisador léxico
	 */
	public Lexer getLexer(CharSource src) {
		Lexer l = new Lexer(this, src);

		for (Token e : tokens) {
			try {
				l.addTokenRule(e.getTokenName(), e.getTokenRule());
			} catch (ReSyntaxException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			}
		}

		return l;
	}

	/**
	 * Checa se a string é o nome de um token válido
	 * 
	 * @param s
	 *            nome a ser checado
	 * @return true se s é o nome de um token
	 */
	public boolean isToken(String s) {
		return tokenSpec.containsKey(s);
	}

	/**
	 * Obtem a especificação léxica para o token de nome s
	 * 
	 * @param s
	 *            nome do token
	 * @return
	 */
	public Token getTokenSpec(String s) {
		return tokenSpec.get(s);
	}

	/**
	 * Cria uma nova instancia do token do tipo especificado por tokenName.
	 * 
	 * @param tokenName
	 *            nome do tipo do token
	 * @param string
	 *            valor da instancia do token
	 * @return nova instancia do tipo do token especificado por tokenName
	 */
	public Lexeme getToken(String tokenName, String string) {
		Token spec = tokenSpec.get(tokenName);

		if (spec == null) {
			throw new SetupException("Token inválido: " + tokenName);
		}

		return new Lexeme(string, spec);
	}
	
	/**
	 * Teste.
	 * @param args
	 * @throws ReSyntaxException 
	 */
	public static void main(String[] args) throws ReSyntaxException {
		LexerSpec spec = new LexerSpec();
		
		spec.addTokenRule("TYPE", "type");
		spec.addTokenRule("VAR", "var");
		spec.addTokenRule("ID", "[a-z]+");
		spec.addTokenRule("SPACE", "[ \r\n]");
		
		Lexer lexer = spec.getLexer(new CharSequenceCharSource("var ehehe type"));
		
		Lexeme l;
		while ((l = lexer.nextLexeme()) != null)
			System.out.println(l);
	}
}
