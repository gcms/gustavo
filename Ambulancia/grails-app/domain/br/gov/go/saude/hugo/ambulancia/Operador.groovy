package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.utilitario.UtilitarioNome

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
        sort 'nome'
        senha column: '`password`'
    }

    Set<Grupo> getAuthorities() {
        OperadorGrupo.findAllByOperador(this).collect { it.grupo } as Set
    }

    String toString() {
        UtilitarioNome.formateNomePessoa(nome)
    }

    boolean equals(Object objeto) {
        if (!Operador.isInstance(objeto))
            return false

        Operador outro = objeto

        outro.usuario == usuario
    }

    int hashCode() {
        usuario.hashCode()
    }
}
