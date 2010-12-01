package br.gov.go.saude.hugo.ambulancia

class GerenciamentoViagemService {
    AmbulanciaService ambulanciaService

    private Viagem obtenhaViagemNaoRetornou(Ambulancia ambulancia) {
        Viagem.findByAmbulanciaAndRetornou(ambulancia, false)
    }

    private Viagem obtenhaViagemNaoRetornou(Motorista motorista) {
        Viagem.findByMotoristaAndRetornou(motorista, false)
    }

    private void verifiqueAtributo(Viagem viagem, String atributo) {
        log.warn "Viagem inválida, '$atributo' em trânsito"

        if (!viagem.errors.hasErrors())
            viagem.validate()

//        if (!viagem.errors.hasFieldErrors(atributo))
        viagem.errors.rejectValue(atributo, 'unique')
    }

    private void verifiqueKmAmbulancia(Viagem viagem, Ambulancia ambulancia) {
        Long kmRetornoUltimo = ambulanciaService.obtenhaKmRetornoUltimaViagem(viagem.ambulancia)

        if (viagem.kmSaida < kmRetornoUltimo) {
            if (!viagem.errors.hasErrors())
                viagem.validate()

            if (!viagem.errors.hasFieldErrors('kmSaida'))
                viagem.errors.rejectValue('kmSaida', 'viagem.kmSaida.min.notmet',
                        ['kmSaida', viagem.getClass().name, viagem.kmSaida, kmRetornoUltimo] as Object[],
                        'O campo [{0}] da classe [{1}] com o valor [{2}] não atinge o valor mínimo [{3}]')
        }
    }

    boolean registreSaida(Viagem viagem) {
        if (obtenhaViagemNaoRetornou(viagem.ambulancia))
            verifiqueAtributo(viagem, 'ambulancia')

        if (obtenhaViagemNaoRetornou(viagem.motorista))
            verifiqueAtributo(viagem, 'motorista')

        if (viagem.ambulancia != null) {
            verifiqueKmAmbulancia(viagem, viagem.ambulancia)
        }

        !viagem.hasErrors() && viagem.save()
    }

    boolean registreRetorno(Viagem viagem) {
        !viagem.hasErrors() && viagem.save()
    }

    Date obtenhaMenorDataSaida() {

    }
}
