package br.gov.go.saude.hugo.ambulancia

import grails.plugins.springsecurity.SpringSecurityService
import org.codehaus.groovy.grails.plugins.springsecurity.GrailsUser

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 17/11/2010
 * Time: 09:42:36
 * To change this template use File | Settings | File Templates.
 */
class GerenciamentoGrupoService {
    SpringSecurityService springSecurityService

    private Grupo crieGrupo(String authority) {
        log.info "Cadastrando grupo $authority ..."

        Grupo grupo = new Grupo(authority: authority)
        grupo.save(flush: true)
        grupo
    }

    public Grupo registreGrupo(String authority) {
        Grupo.findByAuthority(authority) ?: crieGrupo(authority)
    }

    public boolean salveOperador(Operador operador, String password, String passwordConfirm) {
        if (password.empty && passwordConfirm.empty) {
            log.info "Salvando operador... Ambas senhas vazias"
            operador.validate()
        } else if (password != passwordConfirm) {
            log.info "Salvando operador... Senhas diferem"
            operador.validate()
            operador.errors.rejectValue('senha', 'operador.senha.matches.invalid')
        } else {
            operador.senha = springSecurityService.encodePassword(password)
            operador.validate()            
        }

        !operador.hasErrors() && operador.save()
    }

    public boolean crieOperador(Operador operador, String password, String passwordConfirm) {
        crieOperador(operador, password, passwordConfirm, registreGrupo('ROLE_USER'))
    }

    public boolean crieOperador(Operador operador, String password, String passwordConfirm, Grupo grupo) {
        log.info "Cadastrando operador ${operador.usuario} ..."

        salveOperador(operador, password, passwordConfirm) &&
                OperadorGrupo.create(operador, grupo, true)
    }

    public Operador crieOperador(String username, String password, Grupo grupo) {
        Operador operador = new Operador(nome: username.capitalize(), usuario: username, enabled: true)
        crieOperador(operador, password, password, grupo) ? operador : null
    }

    public Operador registreUsuario(String username, String password, String authority) {
        Operador.findByUsuario(username) ?: crieOperador(username, password, registreGrupo(authority))
    }

    public void excluaOperador(Operador operador) {
        log.info "Excluindo operador $operador"
        
        OperadorGrupo.findAllByOperador(operador).each { OperadorGrupo operadorGrupo ->
            operadorGrupo.delete()
        }

        operador.delete()
    }

    public GrailsUser getPrincipal() {
        springSecurityService.loggedIn ? springSecurityService.principal : null
    }

    public Operador getOperadorLogado() {
        principal?.username ? Operador.findByUsuario(principal.username) : null
    }
}
