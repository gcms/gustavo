package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.utilitario.UtilitarioDataHorario

class Viagem {

    Motorista motorista
    Ambulancia ambulancia
    Operador operador

    Date dataSaida
    Date horaSaida
    Long kmSaida

    Date dataRetorno
    Date horaRetorno
    Long kmRetorno

    String observacoes

    List paradas = []

    boolean retornou = false

    Long distancia

    Long getDistancia() {
        distancia ?: (kmRetorno && kmSaida ? kmRetorno - kmSaida : null)
    }

    void setDistancia(Long distancia) {
        this.distancia = null
    }

//    Date dateCreated
//    Date lastUpdated

    static auditable = true

    static mapping = {
        distancia formula: 'km_retorno - km_saida'
        paradas cascade: 'all-delete-orphan', lazy: false
        dataSaida insertable: false, updateable: false, column: 'hora_saida'
        horaSaida column: 'hora_saida'
        dataRetorno insertable: false, updateable: false, column: 'hora_retorno'
        horaRetorno column: 'hora_retorno'
    }

    static hasMany = [paradas: Parada]

    Date getDataSaida() {
        horaSaida
    }

    void setDataSaida(Date data) {
        horaSaida = horaSaida ? UtilitarioDataHorario.default.copieData(horaSaida, data) : data
    }

    void setHoraSaida(Date data) {
        horaSaida = horaSaida ? UtilitarioDataHorario.default.copieHora(horaSaida, data) : data
    }

    Date getDataRetorno() {
        horaRetorno
    }

    void setDataRetorno(Date data) {
        horaRetorno = horaRetorno ? UtilitarioDataHorario.default.copieData(horaRetorno, data) : data
    }

    void setHoraRetorno(Date data) {
        horaRetorno = horaRetorno ? UtilitarioDataHorario.default.copieHora(horaRetorno, data) : data
    }


    static constraints = {
        motorista nullable: false
        ambulancia nullable: false
        operador nullable: false

        horaSaida nullable: false
        kmSaida nullable: false

        dataRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.dataSaida)
                return ['min.notmet', obj.dataSaida ]

            true
        }
        horaRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.horaSaida)
                return ['min.notmet', obj.horaSaida ]

            true
        }
        kmRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.kmSaida)
                return ['min.notmet', obj.kmSaida ]

            true
        }

        distancia nullable: true

        observacoes nullable: true, maxSize: 500

        paradas validator: { val, obj ->
            for (Parada parada : val) {
                if (!parada.validate())
                    return 'blank'
            }

           true
        }
    }

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
