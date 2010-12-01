package br.gov.go.saude.hugo.ambulancia

class Operador {

    String nome
    String telefone

	String usuario
	String senha

	boolean enabled
	boolean accountExpired
	boolean accountLocked
	boolean passwordExpired

	static constraints = {
        nome nullable: true
        telefone nullable: true
        
		usuario blank: false, unique: true
		senha blank: false
	}

	static mapping = {
		senha column: '`password`'
	}

	Set<Grupo> getAuthorities() {
		OperadorGrupo.findAllByOperador(this).collect { it.grupo } as Set
	}

    String toString() {
        nome
    }
}
