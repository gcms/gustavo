package br.ufg.inf.compiler.lexical;

/**
 * Classe representando um token. Um token � visto como uma classe de lexemes,
 * ou seja, um conjunto de cadeias de caracteres que fazem parte de um mesmo
 * grupo. Esta classe de caracteres � especificada por uma regra de forma��o,
 * uma express�o regular.
 * 
 * @author gustavo
 * 
 */
public class Token {
	/** Nome do token. */
	private String tokenName;

	/** Express�o regular, representando regra de forma��o do token. */
	private String tokenRule;

	/**
	 * Cria um novo token.
	 * 
	 * @param tokenName
	 *            nome do token
	 * @param tokenRule
	 *            regra de forma��o do token
	 */
	public Token(String tokenName, String tokenRule) {
		super();
		this.tokenName = tokenName;
		this.tokenRule = tokenRule;
	}

	/**
	 * Obt�m nome do token.
	 * 
	 * @return nome do token
	 */
	public String getTokenName() {
		return tokenName;
	}

	/**
	 * Obt�m regra do token.
	 * 
	 * @return regra do token
	 */
	public String getTokenRule() {
		return tokenRule;
	}

	/**
	 * Retorna uma representa��o do token como texto.
	 * 
	 * @return representa��o textual
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