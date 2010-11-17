import br.gov.go.saude.hugo.ambulancia.GerenciadorGrupoService
import br.gov.go.saude.hugo.ambulancia.Operador
import br.gov.go.saude.hugo.ambulancia.Grupo
import br.gov.go.saude.hugo.ambulancia.GerenciadorGrupoService

class BootStrap {
    GerenciadorGrupoService gerenciadorGrupoService

    def init = { servletContext ->
        Locale.default = new Locale("pt", "BR")

        gerenciadorGrupoService.registreGrupo('ROLE_ADMIN')
        gerenciadorGrupoService.registreGrupo('ROLE_USER')

        gerenciadorGrupoService.registreUsuario('admin', '12345', 'ROLE_ADMIN')

        assert Operador.count() >= 1
        assert Grupo.count() >= 2
    }
    def destroy = {
    }
}
