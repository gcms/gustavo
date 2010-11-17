package br.gov.go.saude.hugo.ambulancia

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 17/11/2010
 * Time: 09:42:36
 * To change this template use File | Settings | File Templates.
 */
class GerenciadorGrupoService {
    def springSecurityService

    private Grupo crieGrupo(String authority) {
        log.info "Cadastrando grupo $authority ..."
        
        Grupo grupo = new Grupo(authority: authority)
        grupo.save(flush: true)
        grupo
    }

    public Grupo registreGrupo(String authority) {
        Grupo.findByAuthority(authority) ?: crieGrupo(authority)
    }

    private Operador crieOperador(String username, String password, Grupo grupo) {
        log.info "Cadastrando operador $username ..."

        Operador operador = new Operador(usuario: username, enabled: true, senha: springSecurityService.encodePassword(password))        
        operador.save(flush: true)

        OperadorGrupo.create operador, grupo, true
        operador
    }

    public Operador registreUsuario(String username, String password, String authority) {
        Operador.findByUsuario(username) ?: crieOperador(username, password, registreGrupo(authority))
    }
}
