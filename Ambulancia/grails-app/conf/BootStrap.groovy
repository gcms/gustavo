import br.gov.go.saude.hugo.ambulancia.GerenciamentoGrupoService
import br.gov.go.saude.hugo.ambulancia.Operador
import br.gov.go.saude.hugo.ambulancia.Grupo
import org.codehaus.groovy.grails.commons.GrailsApplication
import org.codehaus.groovy.grails.commons.GrailsDomainClass

class BootStrap {
    GerenciamentoGrupoService gerenciamentoGrupoService
    GrailsApplication grailsApplication

    // TODO: mover inicialização para partes separadas
    def init = { servletContext ->
        Locale.default = new Locale("pt", "BR")

        gerenciamentoGrupoService.registreGrupo('ROLE_ADMIN')
        gerenciamentoGrupoService.registreGrupo('ROLE_USER')

        gerenciamentoGrupoService.registreUsuario('admin', '12345', 'ROLE_ADMIN')

        assert Operador.count() >= 1
        assert Grupo.count() >= 2


        grailsApplication.domainClasses.each() { GrailsDomainClass dc ->
            dc.clazz.metaClass.getRealClass = {
                org.hibernate.Hibernate.getClass(delegate)
            }            
        }

        String.metaClass.uncapitalize = {
            org.apache.commons.lang.StringUtils.uncapitalize(delegate)
        }
    }


    def destroy = {
    }
}
