package br.ufg.inf.compiler.lexical;

import java.util.HashMap;
import java.util.Map;

import br.ufg.inf.compiler.syntatic.Terminal;

import monq.jfa.CharSource;
import monq.jfa.Nfa;
import monq.jfa.ReSyntaxException;

/**
 * Classe utilizada para representar uma especificação léxico. Esta classe é
 * usada posteriormente para gerar um analisador léxico, capaz de examinar uma
 * sequência de caracteres e convertê-la para uma sequência de tokens.
 * 
 * @author gustavomota
 * 
 */
public class LexerSpec {
	private Map<String, String> tokenSpec;

	private Nfa nfa;

	/**
	 * Cria uma nova especificação léxica.
	 */
	public LexerSpec() {
		tokenSpec = new HashMap<String, String>();
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
		tokenSpec.put(tokenName, tokenRule);
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
		Lexer l = new Lexer(src);

		for (Map.Entry<String, String> e : tokenSpec.entrySet()) {
			try {
				l.addTokenRule(e.getKey(), e.getValue());
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

	public Terminal getTerminal(String s) {
		return new Terminal(s);
	}
}
