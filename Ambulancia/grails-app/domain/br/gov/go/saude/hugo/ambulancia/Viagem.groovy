package br.gov.go.saude.hugo.ambulancia

class Viagem {

    Motorista motorista
    Ambulancia ambulancia
    Operador operador
    
    Date horaSaida
    Integer kmSaida

    Date horaRetorno
    Integer kmRetorno

    String observacoes

    String destino
    List pacientes = []

    boolean retornou = false

    static validadorDadosRetorno = { val, obj ->
        val != null || !obj.retornou
    }

    static constraints = {
        motorista(nullable: false)
        ambulancia(nullable: false)
        operador(nullable: false)

        horaSaida(nullable: false)
        kmSaida(nullable: false)

        horaRetorno(nullable: true, validator: validadorDadosRetorno)
        kmRetorno(nullable: true, validator: validadorDadosRetorno)

        destino(nullable: true)
        observacoes(nullable: true)
    }


    static hasMany = [pacientes: String]

    void registreSaida(Date horaSaida, int kmSaida) {
        this.horaSaida = horaSaida
        this.kmSaida = kmSaida
    }

    void registreRetorno(Date horaRetorno, int kmRetorno) {
        this.horaRetorno = horaRetorno
        this.kmRetorno = kmRetorno
        retornou = true
    }

    boolean jaRetornou() {
        retornou
    }

}
