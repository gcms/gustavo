package br.gov.go.saude.hugo.ambulancia

import org.codehaus.groovy.grails.web.servlet.mvc.GrailsParameterMap

class OperadorController {
    GerenciamentoGrupoService gerenciamentoGrupoService

    def index = { redirect(action: "list", params: params) }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [save: "POST", update: "POST", delete: "POST"]

    def list = {
        params.max = Math.min(params.max ? params.max.toInteger() : 10,  100)
        [operadorInstanceList: Operador.list(params), operadorInstanceTotal: Operador.count()]
    }

    def create = {
        def operadorInstance = new Operador(params)
        return [operadorInstance: operadorInstance, grupos: Grupo.list().reverse()]
    }

    def save = {
        Grupo grupo = Grupo.get(params.grupo)

        def operadorInstance = new Operador(params)
        if (gerenciamentoGrupoService.crieOperador(operadorInstance, params.senha, params.senhaConfirmada, grupo)) {
            flash.message = "operador.created"
            flash.args = [operadorInstance.id]
            flash.defaultMessage = "Operador ${operadorInstance.id} created"
            redirect(action: "show", id: operadorInstance.id)
        }
        else {
            render(view: "create", model: [operadorInstance: operadorInstance, grupo: params.grupo, grupos: Grupo.list().reverse()])
        }
    }

    def show = {
        def operadorInstance = Operador.get(params.id)
        if (!operadorInstance) {
            flash.message = "operador.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Operador not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [operadorInstance: operadorInstance]
        }
    }

    def edit = {
        def operadorInstance = Operador.get(params.id)
        if (!operadorInstance) {
            flash.message = "operador.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Operador not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [operadorInstance: operadorInstance]
        }
    }

    def update = {
        def operadorInstance = Operador.get(params.id)
        if (operadorInstance) {
            String senha = params.remove('senha')
            String senhaConfirmada = params.remove('senhaConfirmada')
          
            operadorInstance.properties = params

            if (gerenciamentoGrupoService.salveOperador(operadorInstance, senha, senhaConfirmada)) {
                flash.message = "operador.updated"
                flash.args = [params.id]
                flash.defaultMessage = "Operador ${params.id} updated"
                redirect(action: "show", id: operadorInstance.id)
            }
            else {
                render(view: "edit", model: [operadorInstance: operadorInstance])
            }
        }
        else {
            flash.message = "operador.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Operador not found with id ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def delete = {
        def operadorInstance = Operador.get(params.id)
        if (operadorInstance) {
            try {
                gerenciamentoGrupoService.excluaOperador(operadorInstance)
//                operadorInstance.delete()
                flash.message = "operador.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Operador ${params.id} deleted"
                redirect(action: "list")
            }
            catch (org.springframework.dao.DataIntegrityViolationException e) {
                flash.message = "operador.not.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Operador ${params.id} could not be deleted"
                redirect(action: "show", id: params.id)
            }
        }
        else {
            flash.message = "operador.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Operador not found with id ${params.id}"
            redirect(action: "list")
        }
    }
}
