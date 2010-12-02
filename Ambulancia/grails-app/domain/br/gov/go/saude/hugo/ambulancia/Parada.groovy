package br.gov.go.saude.hugo.ambulancia

class Parada {
    String destino

    static belongsTo = Viagem

    static constraints = {
        destino(blank: false)
    }
}
