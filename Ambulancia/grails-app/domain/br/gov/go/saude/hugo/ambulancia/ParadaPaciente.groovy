package br.gov.go.saude.hugo.ambulancia

class ParadaPaciente extends Parada {
    String paciente
    String aih
    
    static constraints = {
        aih(nullable: true)
    }

    String toString() {
        "$paciente - $destino"
    }
}