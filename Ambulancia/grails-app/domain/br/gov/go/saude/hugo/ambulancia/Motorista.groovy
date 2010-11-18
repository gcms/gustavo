package br.gov.go.saude.hugo.ambulancia

class Motorista {
    String nome

    static constraints = {
        nome(blank: false, unique: true)
    }

    String toString() {
        nome
    }
}
