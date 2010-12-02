package br.gov.go.saude.hugo.ambulancia

import org.springframework.context.MessageSource

class GerenciamentoViagemService {
    AmbulanciaService ambulanciaService
    MessageSource messageSource

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

        viagem.errors.rejectValue(atributo, 'unique',
                [atributo, viagem.getClass().name, viagem."$atributo"] as Object[],
                messageSource.getMessage('default.not.unique.message', [] as Object[], Locale.default))
    }

    private void verifiqueKmAmbulancia(Viagem viagem, Ambulancia ambulancia) {
        Long kmRetornoUltimo = ambulanciaService.obtenhaKmRetornoUltimaViagem(viagem.ambulancia)

        if (viagem.kmSaida < kmRetornoUltimo) {
            if (!viagem.errors.hasErrors())
                viagem.validate()

//            if (!viagem.errors.hasFieldErrors('kmSaida'))
                viagem.errors.rejectValue('kmSaida', 'viagem.kmSaida.min.notmet',
                        ['kmSaida', viagem.getClass().name, viagem.kmSaida, kmRetornoUltimo] as Object[],
                        messageSource.getMessage('default.invalid.min.message', [] as Object[], Locale.default))
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
}
