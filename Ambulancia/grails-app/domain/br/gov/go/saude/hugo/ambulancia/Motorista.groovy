package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.utilitario.UtilitarioNome

class Motorista {
    String nome
    String telefone

    Boolean disponivel = true

    static constraints = {
        nome blank: false, unique: true
        telefone nullable: true
    }

    String toString() {
        UtilitarioNome.formateNomePessoa(nome)
    }
}
