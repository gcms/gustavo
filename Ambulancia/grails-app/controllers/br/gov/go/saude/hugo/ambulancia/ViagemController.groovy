package br.gov.go.saude.hugo.ambulancia

import grails.plugins.springsecurity.SpringSecurityService

class ViagemController {
    MotoristaService motoristaService
    AmbulanciaService ambulanciaService
    SpringSecurityService springSecurityService
    GerenciamentoGrupoService gerenciamentoGrupoService
    GerenciamentoViagemService gerenciamentoViagemService

    def index = { redirect(action: "list", params: params) }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [save: "POST", update: "POST", delete: "POST"]

    def list = {
        params.max = Math.min(params.max ? params.max.toInteger() : 10,  100)

        def criteria = { eq('retornou', false) }

        List viagens = Viagem.createCriteria().list(criteria)
        long count = Viagem.createCriteria().count(criteria)

        [viagemInstanceList: viagens, viagemInstanceTotal: count]
    }

    def create = {
        List motoristas = motoristaService.obtenhaMotoristasDisponiveis()
        List ambulancias = ambulanciaService.obtenhaAmbulanciasDisponiveis()

        Viagem viagem = flash.viagem ?:new Viagem(params)

        [viagemInstance: viagem, motoristas: motoristas, ambulancias: ambulancias]
    }

    def save = {
        Viagem viagem = new Viagem(params)
        viagem.operador = gerenciamentoGrupoService.obtenhaOperadorLogado()

        if (gerenciamentoViagemService.registreSaida(viagem)) {
            flash.message = "viagem.created"
            flash.args = [viagem.id]
            flash.defaultMessage = "Viagem ${viagem.id} created"
            //redirect(action: "show", id: viagem.id)
            redirect(action: 'list')
        } else {
            flash.viagem = viagem
            redirect(action: "create")
        }
    }

    def show = {
        def viagemInstance = Viagem.get(params.id)
        if (!viagemInstance) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Viagem not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [viagemInstance: viagemInstance]
        }
    }

    def edit = {
        def viagemInstance = flash.viagem ?: Viagem.get(params.id)
        if (!viagemInstance) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Viagem not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [viagemInstance: viagemInstance]
        }
    }

    def update = {
        def viagemInstance = Viagem.get(params.id)
        if (viagemInstance) {
            viagemInstance.properties = params
            viagemInstance.retornou = true

            if (gerenciamentoViagemService.registreRetorno(viagemInstance)) {
                flash.message = "viagem.updated"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem ${params.id} updated"
                redirect(action: "show", id: viagemInstance.id)
            } else {
                flash.viagem = viagemInstance
                redirect(action: 'edit', params: [id: params.id])
            }
        }
        else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Viagem not found with id ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def delete = {
        def viagemInstance = Viagem.get(params.id)
        if (viagemInstance) {
            try {
                viagemInstance.delete()
                flash.message = "viagem.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem ${params.id} deleted"
                redirect(action: "list")
            }
            catch (org.springframework.dao.DataIntegrityViolationException e) {
                flash.message = "viagem.not.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem ${params.id} could not be deleted"
                redirect(action: "show", id: params.id)
            }
        }
        else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Viagem not found with id ${params.id}"
            redirect(action: "list")
        }
    }
}
