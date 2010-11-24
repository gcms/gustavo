package br.gov.go.saude.hugo.ambulancia

class Viagem {

    Motorista motorista
    Ambulancia ambulancia
    Operador operador
    
    Date horaSaida
    Long kmSaida

    Date horaRetorno
    Long kmRetorno

    String observacoes

    String destino
    List pacientes = []

    boolean retornou = false

    Long distancia

    Long getDistancia() {
        distancia ?: (kmRetorno ? kmRetorno - kmSaida : null)
    }

    static mapping = {
        distancia formula: 'km_retorno - km_saida'
    }

    static transients = [ 'dataSaida', 'dataRetorno' ]

    Date getDataSaida() {
        horaSaida
    }

    void setDataSaida(Date data) {
        horaSaida = horaSaida ? UtilitarioDataHorario.copieData(horaSaida, data) : data
    }

    void setHoraSaida(Date data) {
        horaSaida = horaSaida ? UtilitarioDataHorario.copieHora(horaSaida, data) : data
    }
    
    Date getDataRetorno() {
        horaRetorno
    }

    void setDataRetorno(Date data) {
        horaRetorno = horaRetorno ? UtilitarioDataHorario.copieData(horaRetorno, data) : data
    }

    void setHoraRetorno(Date data) {
        horaRetorno = horaRetorno ? UtilitarioDataHorario.copieHora(horaRetorno, data) : data
    }


    static constraints = {
        motorista(nullable: false)
        ambulancia(nullable: false)
        operador(nullable: false)

        horaSaida(nullable: false)
        kmSaida(nullable: false)

        horaRetorno(nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.horaSaida)
                return 'min'
        })
        kmRetorno(nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.kmSaida)
                return 'min'
        })

        distancia(nullable: true)

        destino(nullable: true, maxSize: 255)
        observacoes(nullable: true, maxSize: 500)
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
