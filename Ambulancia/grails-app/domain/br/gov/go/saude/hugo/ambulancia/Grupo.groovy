package br.gov.go.saude.hugo.ambulancia

class Grupo {
    
	String authority

	static mapping = {
		cache true
	}

	static constraints = {
		authority blank: false, unique: true
	}

    String toString() {
        authority
    }
}
