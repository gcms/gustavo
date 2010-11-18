package br.gov.go.saude.hugo.ambulancia

class GerenciamentoViagemService {

    static transactional = true

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

        if (!viagem.errors.hasFieldErrors(atributo))
            viagem.errors.rejectValue(atributo, 'unique')
    }

    void registreSaida(Viagem viagem) {
        if (obtenhaViagemNaoRetornou(viagem.ambulancia))
            verifiqueAtributo(viagem, 'ambulancia')

        if (obtenhaViagemNaoRetornou(viagem.motorista))
            verifiqueAtributo(viagem, 'motorista')

        if (!viagem.hasErrors())
            viagem.save()
    }

    void registreRetorno(Viagem viagem) {
        viagem.save()
    }
}
