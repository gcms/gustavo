package br.ufg.inf.compiler.syntatic;

import br.ufg.inf.compiler.lexical.Lexeme;

/**
 * Define uma interface usada pela tabela sint�tica para obter Tokens.
 * 
 * @author gustavo
 * 
 */
public interface TokenStream {
	public Lexeme getToken();

	public void advance();
}
