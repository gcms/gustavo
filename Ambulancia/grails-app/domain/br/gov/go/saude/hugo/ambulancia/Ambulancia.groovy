package br.gov.go.saude.hugo.ambulancia

class Ambulancia {

    String prefixo
    String placa
    String descricao

    Boolean disponivel = true

    static constraints = {
        prefixo blank: false, unique: true, matches: '[0-9]+', maxSize: 4
        placa blank: false, unique: true, matches: '[A-Z]{3}\\-[0-9]{4}'
        descricao nullable: true
    }

    void setPlaca(String placa) {
        this.placa = placa.toUpperCase()
    }

    String toString() {
        "$prefixo - $placa"
    }

    boolean equals(Object objeto) {
        if (!Ambulancia.isInstance(objeto))
            return false

        Ambulancia outra = objeto

        outra.prefixo == prefixo && outra.placa == placa
    }

    int hashCode() {
        prefixo.hashCode() ^ 3 + placa.hashCode() ^ 5
    }
}
