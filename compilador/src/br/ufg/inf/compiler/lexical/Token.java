package br.ufg.inf.compiler.lexical;

/**
 * Classe utilizada para representar um token. Aqui um token consiste em um
 * valor (string de caracteres lida) e um tipo, que identifica a que classe de
 * cadeias o token pertence.
 * 
 * @author gustavomota
 * 
 */
public class Token {
	/**
	 * Cria um novo token.
	 * 
	 * @param manager
	 *            gerenciador de tokens
	 * @param tokenId
	 *            identificador do tipo do token
	 * @param value
	 *            valor do token
	 */
	Token(TokenManager manager, Integer tokenId, String value) {
		this.manager = manager;
		this.tokenId = tokenId;
		this.value = value;
	}

	/** Gerenciador de tokens associado. */
	private TokenManager manager;

	/** Identificador unico associado ao manager, indicando o tipo do token. */
	private Integer tokenId;

	/** String contendo a cadeia de caracteres correspondente a esse token. */
	private String value;

	/**
	 * Obtem o valor deste token.
	 * 
	 * @return valor do token
	 */
	public String getValue() {
		return value;
	}

	/**
	 * Obtem representacao textual do tipo do token.
	 * 
	 * @return String contendo o tipo do token
	 */
	public String getType() {
		return manager.getTokenType(tokenId);
	}

	/**
	 * Obtem representacao textual do token.
	 * 
	 * @return String representando o token
	 */
	public String toString() {
		return getType() + "\t" + getValue();
	}

	/**
	 * Verifica se um outro token eh igual a este token.
	 * 
	 * @return true se o e igual a este token
	 */
	public boolean equals(Object o) {
		if (!(o instanceof Token))
			return false;

		Token t = (Token) o;

		return t.tokenId == tokenId && t.value.equals(value);
	}

	/**
	 * Obtem codigo hash para este token.
	 * 
	 * @return hashcode deste token
	 */
	public int hashCode() {
		return value.hashCode() + tokenId;
	}
}
