package br.gov.go.saude.hugo.ambulancia

class Motorista {
    String nome
    String telefone

    static constraints = {
        nome blank: false, unique: true
        telefone nullable: true
    }

    String toString() {
        nome
    }
}
