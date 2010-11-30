import br.gov.go.saude.hugo.ambulancia.GerenciamentoGrupoService
import br.gov.go.saude.hugo.ambulancia.Operador
import br.gov.go.saude.hugo.ambulancia.Grupo

class BootStrap {
    GerenciamentoGrupoService gerenciamentoGrupoService

    def init = { servletContext ->
        Locale.default = new Locale("pt", "BR")

        gerenciamentoGrupoService.registreGrupo('ROLE_ADMIN')
        gerenciamentoGrupoService.registreGrupo('ROLE_USER')

        gerenciamentoGrupoService.registreUsuario('admin', '12345', 'ROLE_ADMIN')

        assert Operador.count() >= 1
        assert Grupo.count() >= 2
    }
    def destroy = {
    }
}
