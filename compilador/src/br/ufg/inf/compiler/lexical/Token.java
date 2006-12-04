package br.ufg.inf.compiler.lexical;

/**
 * Classe representando um token. Um token é visto como uma classe de lexemes,
 * ou seja, um conjunto de cadeias de caracteres que fazem parte de um mesmo
 * grupo. Esta classe de caracteres é especificada por uma regra de formação,
 * uma expressão regular.
 * 
 * @author gustavo
 * 
 */
public class Token {
	/** Nome do token. */
	private String tokenName;

	/** Expressão regular, representando regra de formação do token. */
	private String tokenRule;

	/**
	 * Cria um novo token.
	 * 
	 * @param tokenName
	 *            nome do token
	 * @param tokenRule
	 *            regra de formação do token
	 */
	public Token(String tokenName, String tokenRule) {
		super();
		this.tokenName = tokenName;
		this.tokenRule = tokenRule;
	}

	/**
	 * Obtém nome do token.
	 * 
	 * @return nome do token
	 */
	public String getTokenName() {
		return tokenName;
	}

	/**
	 * Obtém regra do token.
	 * 
	 * @return regra do token
	 */
	public String getTokenRule() {
		return tokenRule;
	}

	/**
	 * Retorna uma representação do token como texto.
	 * 
	 * @return representação textual
	 */
	public String toString() {
		return getTokenName() + "(" + getTokenRule() + ")";
	}

	@Override
	public int hashCode() {
		final int PRIME = 31;
		int result = 1;
		result = PRIME * result
				+ ((tokenName == null) ? 0 : tokenName.hashCode());
		result = PRIME * result
				+ ((tokenRule == null) ? 0 : tokenRule.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		final Token other = (Token) obj;
		if (tokenName == null) {
			if (other.tokenName != null)
				return false;
		} else if (!tokenName.equals(other.tokenName))
			return false;
		if (tokenRule == null) {
			if (other.tokenRule != null)
				return false;
		} else if (!tokenRule.equals(other.tokenRule))
			return false;
		return true;
	}

}