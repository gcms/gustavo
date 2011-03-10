import br.gov.go.saude.hugo.ambulancia.GerenciamentoGrupoService
import grails.plugins.springsecurity.SpringSecurityService

class AmbulanciaFilters {
    GerenciamentoGrupoService gerenciamentoGrupoService
    SpringSecurityService springSecurityService

    def filters = {

        all(controller:'*', action:'*') {
            before = {
                params.offset = params.offset ? params.offset.toInteger() : 0
                params.max = Math.min(params.max ? params.max.toInteger() : 10, 100)

                if (springSecurityService.isLoggedIn() && session.login == null) {
                    session.login = new Date()

                    this.log.info "Usuário $gerenciamentoGrupoService.operadorLogado entrou no sistema..."
                }

            }

            after = { model ->
                if (model)
                    model.operador = gerenciamentoGrupoService.operadorLogado 
            }
        }
    }
}