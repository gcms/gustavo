package br.ufg.inf.compiler.lexical;

public class Token {
	Token(TokenManager manager, Integer tokenId, String value) {
		this.manager = manager;
		this.tokenId = tokenId;
		this.value = value;
	}

	private TokenManager manager;

	private Integer tokenId;

	private String value;

	public String getValue() {
		return value;
	}

	public String getType() {
		return manager.getTokenType(tokenId);
	}

	public String toString() {
		return getType() + "\t" + getValue();
	}

	public boolean equals(Object o) {
		if (!(o instanceof Token))
			return false;

		Token t = (Token) o;

		return t.tokenId == tokenId && t.value.equals(value);
	}

	public int hashCode() {
		return value.hashCode() + tokenId;
	}
}
