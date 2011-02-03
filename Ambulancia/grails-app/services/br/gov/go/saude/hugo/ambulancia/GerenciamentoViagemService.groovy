package br.gov.go.saude.hugo.ambulancia

import org.springframework.context.MessageSource
import org.springframework.validation.Errors

class GerenciamentoViagemService {
    AmbulanciaService ambulanciaService
    MessageSource messageSource

    public void verifiqueAmbulancia(Viagem viagem) {
        verifiqueAtributo(viagem, 'ambulancia')
    }

    public void verifiqueMotorista(Viagem viagem) {
        verifiqueAtributo(viagem, 'motorista')
    }

    public void verifiqueAtributo(Viagem viagem, String atributo) {
        if (obtenhaViagemNaoRetornou(viagem, atributo) != viagem)
            marqueAtributoInvalido(viagem, atributo)
    }

    private Viagem obtenhaViagemNaoRetornou(Viagem viagem, String atributo) {
        String hql = "from Viagem where ${atributo}.id = ? and ((? between horaSaida and horaRetorno) or (? >= horaSaida and retornou = false)) order by horaSaida"
        List viagens = Viagem.executeQuery(hql, [ viagem."$atributo".id, viagem.horaSaida, viagem.horaSaida])

        if (viagens.size() > 1) {
            log.error "Não devem haver viagens com mesmo motorista ou ambulancia com horarios em interesecção"
        }

        viagens.empty ? viagem : viagens.last()
    }

    private void marqueAtributoInvalido(Viagem viagem, String atributo) {
        log.warn "Viagem inválida, '$atributo' em trânsito"

        viagem.errors.rejectValue(atributo, 'unique',
                [atributo, viagem.getClass().name, viagem."$atributo"] as Object[],
                messageSource.getMessage('default.not.unique.message', [] as Object[], Locale.default))
    }

    public void verifiqueKmSaida(Viagem viagem) {
        verifiqueKmSaida(viagem.kmSaida, viagem, viagem.errors)
    }

    public void verifiqueKmSaida(Long kmSaida, Viagem viagem, Errors errors) {
        Long kmUltimoRetorno = ambulanciaService.obtenhaKmRetornoUltimaViagem(viagem.ambulancia, viagem.horaSaida)

        if (kmSaida < kmUltimoRetorno) {
            log.warn "kmSaida inválida kmSaida ($kmSaida) < kmUltimoRetorno ($kmUltimoRetorno) em $viagem.horaSaida"

            errors.rejectValue('kmSaida', 'viagem.kmSaida.min.notmet',
                    ['kmSaida', viagem.getClass().name, kmSaida, kmUltimoRetorno] as Object[],
                    messageSource.getMessage('default.invalid.min.message', [] as Object[], Locale.default))
        }
    }

    boolean registreSaida(Viagem viagem) {
        log.info "Registrando saida $viagem"
        atualizeViagem(viagem)
    }

    boolean registreRetorno(Viagem viagem) {
        log.info "Registrando retorno $viagem"
        viagem.retornou = true
        atualizeViagem(viagem)
    }

    boolean atualizeViagem(Viagem viagem) {
        viagem.validate() && viagem.save(flush: true)
    }
}
