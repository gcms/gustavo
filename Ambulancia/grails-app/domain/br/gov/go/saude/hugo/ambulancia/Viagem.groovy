package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.utilitario.UtilitarioDataHorario
import org.springframework.validation.Errors

class Viagem {
    def gerenciamentoViagemService

    Motorista motorista
    Ambulancia ambulancia

    Date dataSaida
    Date horaSaida
    Long kmSaida
    Operador operador

    Date dataRetorno
    Date horaRetorno
    Long kmRetorno
    Operador operadorRetorno

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

    static auditable = true

    static mapping = {
        distancia formula: 'km_retorno - km_saida'
        paradas cascade: 'all-delete-orphan', lazy: false

        dataSaida insertable: false, updateable: false, column: 'hora_saida'
        horaSaida column: 'hora_saida'
        kmSaida column: 'km_saida'

        dataRetorno insertable: false, updateable: false, column: 'hora_retorno'
        horaRetorno column: 'hora_retorno'
        kmRetorno column: 'km_retorno'
    }

    static hasMany = [paradas: Parada]

    Date getDataSaida() {
        horaSaida
    }

    void setDataSaida(Date data) {
        if (data && horaSaida) {
            horaSaida = UtilitarioDataHorario.copieData(horaSaida, data)
        } else if (horaSaida) {
            horaSaida = UtilitarioDataHorario.copieData(horaSaida, new Date())
        } else {
            horaSaida = data
        }
    }

    void setHoraSaida(Date hora) {
        if (horaSaida) {
            horaSaida = UtilitarioDataHorario.copieData(hora, horaSaida)
        } else {
            horaSaida = hora
        }
    }

    Date getDataRetorno() {
        horaRetorno
    }

    void setDataRetorno(Date data) {
        if (data && horaRetorno) {
            horaRetorno = UtilitarioDataHorario.copieData(horaRetorno, data)
        } else if (horaRetorno) {
            horaRetorno = UtilitarioDataHorario.copieData(horaRetorno, new Date())
        } else {
            horaRetorno = data
        }
    }

    void setHoraRetorno(Date hora) {
        if (horaRetorno) {
            horaRetorno = UtilitarioDataHorario.copieData(hora, horaRetorno)
        } else {
            horaRetorno = hora
        }        
    }

    static constraints = {
        motorista nullable: false, validator: { Motorista motorista, Viagem viagem, Errors errors ->
            viagem.gerenciamentoViagemService?.verifiqueMotorista(viagem)
        }
        ambulancia nullable: false, validator: { Ambulancia ambulancia, Viagem viagem, Errors errors ->
            viagem.gerenciamentoViagemService?.verifiqueAmbulancia(viagem)
        }

        operador nullable: false

        horaSaida nullable: false
        kmSaida nullable: false, validator: { Long kmSaida, Viagem viagem, Errors errors ->
            viagem.gerenciamentoViagemService?.verifiqueKmSaida(viagem)
        }

        dataRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.dataSaida)
                return ['min.notmet', obj.dataSaida]

            true
        }
        horaRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.horaSaida)
                return ['min.notmet', obj.horaSaida]

            true
        }
        kmRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'
            if (val != null && val < obj.kmSaida)
                return ['min.notmet', obj.kmSaida]

            true
        }
        operadorRetorno nullable: true, validator: { val, obj ->
            if (val == null && obj.retornou)
                return 'nullable'

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

    void registreSaida(Operador operador, Date horaSaida, Long kmSaida) {
        this.operador = operador
        setDataSaida(horaSaida)
        setHoraSaida(horaSaida)
        this.kmSaida = kmSaida
    }

    void registreRetorno(Operador operador, Date horaRetorno, Long kmRetorno) {
        this.operadorRetorno = operador
        setDataRetorno(horaRetorno)
        setHoraRetorno(horaRetorno)
        this.kmRetorno = kmRetorno
        retornou = true
    }

    boolean jaRetornou() {
        retornou
    }
}
