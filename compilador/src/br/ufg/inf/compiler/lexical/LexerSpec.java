package br.ufg.inf.compiler.lexical;

import java.util.HashMap;
import java.util.Map;

import br.ufg.inf.compiler.main.SetupException;

import monq.jfa.CharSource;
import monq.jfa.Nfa;
import monq.jfa.ReSyntaxException;

/**
 * Classe utilizada para representar uma especifica��o l�xico. Esta classe �
 * usada posteriormente para gerar um analisador l�xico, capaz de examinar uma
 * sequ�ncia de caracteres e convert�-la para uma sequ�ncia de tokens.
 * 
 * @author gustavomota
 * 
 */
public class LexerSpec {

	private Map<String, Token> tokenSpec;

	private Nfa nfa;

	/**
	 * Cria uma nova especifica��o l�xica.
	 */
	public LexerSpec() {
		tokenSpec = new HashMap<String, Token>();

		nfa = new Nfa(Nfa.NOTHING);
	}

	/**
	 * Adiciona uma regra l�xica a especifica��o.
	 * 
	 * @param tokenName
	 *            nome da regra l�xica
	 * @param tokenRule
	 *            express�o regular da regra
	 * @throws ReSyntaxException
	 */
	public void addTokenRule(final String tokenName, String tokenRule)
			throws ReSyntaxException {
		tokenSpec.put(tokenName, new Token(tokenName, tokenRule));
		nfa.or(tokenRule);
	}

	/**
	 * Cria um analisador l�xico para a fonte de caracteres especificada.
	 * 
	 * @param src
	 *            fonte de caracteres
	 * @return um novo analisador l�xico
	 */
	public Lexer getLexer(CharSource src) {
		Lexer l = new Lexer(this, src);

		for (Token e : tokenSpec.values()) {
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
	 * Checa se a string � o nome de um token v�lido
	 * 
	 * @param s
	 *            nome a ser checado
	 * @return true se s � o nome de um token
	 */
	public boolean isToken(String s) {
		return tokenSpec.containsKey(s);
	}

	/**
	 * Obtem a especifica��o l�xica para o token de nome s
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
			throw new SetupException("Token inv�lido: " + tokenName);
		}

		return new Lexeme(string, spec);
	}
}
