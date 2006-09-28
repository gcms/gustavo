package br.ufg.inf.compiler.lexical;

import java.util.HashMap;
import java.util.Map;

public class TokenManager {
	private Map<String, Integer> typeToId;
	private Map<Integer, String> idToType;
	
	private int newId;
	
	public TokenManager() {
		typeToId = new HashMap<String, Integer>();
		idToType = new HashMap<Integer, String>();
		
		newId = 0;
	}
	
	public Token getToken(String tokenType, String tokenValue) {
		Integer tokenId = typeToId.get(tokenType);
		if (tokenId == null) {
			tokenId = ++newId;
			typeToId.put(tokenType, tokenId);
			idToType.put(tokenId, tokenType);
		}
		
		return new Token(this, tokenId, tokenValue);
	}
	
	String getTokenType(int tokenId) {
		return idToType.get(tokenId);
	}

}
