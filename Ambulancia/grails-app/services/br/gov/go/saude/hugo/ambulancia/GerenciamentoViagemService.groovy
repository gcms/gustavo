package br.gov.go.saude.hugo.ambulancia

import org.springframework.context.MessageSource

class GerenciamentoViagemService {
    AmbulanciaService ambulanciaService
    MessageSource messageSource

    private Viagem obtenhaViagemNaoRetornou(Viagem viagem, String atributo) {
        Viagem.findWhere(["${atributo}": viagem."$atributo", retornou: false]) ?: viagem
    }

    private boolean ambulanciaNaoRetornou(Viagem viagem) {
        obtenhaViagemNaoRetornou(viagem, 'ambulancia') != viagem
    }

    private boolean motoristaNaoRetornou(Viagem viagem) {
        obtenhaViagemNaoRetornou(viagem, 'motorista') != viagem
    }    

    private void marqueAtributoInvalido(Viagem viagem, String atributo) {
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
        log.info "Registrando saida $viagem"

        if (ambulanciaNaoRetornou(viagem))
            marqueAtributoInvalido(viagem, 'ambulancia')

        if (motoristaNaoRetornou(viagem))
            marqueAtributoInvalido(viagem, 'motorista')

        if (viagem.ambulancia != null) {
            verifiqueKmAmbulancia(viagem, viagem.ambulancia)
        }

        !viagem.hasErrors() && viagem.save()
    }

    boolean registreRetorno(Viagem viagem) {
        log.info "Registrando retorno $viagem"
        viagem.retornou = true
        
        !viagem.hasErrors() && viagem.save()
    }
}
