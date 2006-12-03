package br.ufg.inf.compiler.lexical;

/**
 * Classe utilizada para representar uma inst�ncia do token. Aqui uma instancia
 * de token consiste em um valor (string de caracteres lida) e uma especifica��o
 * do token, que identifica a que classe de cadeias este lexema pertence.
 * 
 * @author gustavomota
 * 
 */
public class Lexeme {
	/** Valor obtido da entrada decaracteres. */
	private String value;

	/** Especifica��o do token desse Lexeme. */
	private Token spec;

	/**
	 * Cria um novo lexeme.
	 * 
	 * @param value
	 *            string com o conte�do em texto do lexeme
	 * @param spec
	 *            especifica��o do token correspondente ao lexeme
	 */
	public Lexeme(String value, Token spec) {
		super();
		this.value = value;
		this.spec = spec;
	}

	/**
	 * Obt�m o token deste lexeme.
	 * 
	 * @return token
	 */
	public Token getTokenSpec() {
		return spec;
	}

	/**
	 * Obt�m o nome do token deste lexeme.
	 * 
	 * @return nome do token
	 */
	public String getTokenName() {
		return getTokenSpec().getTokenName();
	}

	/**
	 * Obt�m o valor deste lexeme.
	 * 
	 * @return valor do lexeme
	 */
	public String getValue() {
		return value;
	}

	@Override
	public int hashCode() {
		final int PRIME = 31;
		int result = 1;
		result = PRIME * result + ((spec == null) ? 0 : spec.hashCode());
		result = PRIME * result + ((value == null) ? 0 : value.hashCode());
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
		final Lexeme other = (Lexeme) obj;
		if (spec == null) {
			if (other.spec != null)
				return false;
		} else if (!spec.equals(other.spec))
			return false;
		if (value == null) {
			if (other.value != null)
				return false;
		} else if (!value.equals(other.value))
			return false;
		return true;
	}
}
