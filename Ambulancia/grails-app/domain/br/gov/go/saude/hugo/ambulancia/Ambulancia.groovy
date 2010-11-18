package br.gov.go.saude.hugo.ambulancia

class Ambulancia {

    String descricao
    String placa

    static constraints = {
        descricao(nullable: true)
        placa(unique: true, matches: '[A-Z]{3}\\-[0-9]{4}')
    }

    void setPlaca(String placa) {
        this.placa = placa.toUpperCase()
    }

    String toString() {
        placa
    }
}
