package br.gov.go.saude.hugo.ambulancia

class AmbulanciaController {

    def index = { redirect(action: "list", params: params) }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [save: "POST", update: "POST", delete: "POST"]

    def list = {
        params.max = Math.min(params.max ? params.max.toInteger() : 10,  100)
        [ambulanciaInstanceList: Ambulancia.list(params), ambulanciaInstanceTotal: Ambulancia.count()]
    }

    def create = {
        def ambulanciaInstance = new Ambulancia()
        ambulanciaInstance.properties = params
        return [ambulanciaInstance: ambulanciaInstance]
    }

    def save = {
        def ambulanciaInstance = new Ambulancia(params)
        if (!ambulanciaInstance.hasErrors() && ambulanciaInstance.save()) {
            flash.message = "ambulancia.created"
            flash.args = [ambulanciaInstance.id]
            flash.defaultMessage = "Ambulancia ${ambulanciaInstance.id} created"
            redirect(action: "show", id: ambulanciaInstance.id)
        }
        else {
            render(view: "create", model: [ambulanciaInstance: ambulanciaInstance])
        }
    }

    def show = {
        def ambulanciaInstance = Ambulancia.get(params.id)
        if (!ambulanciaInstance) {
            flash.message = "ambulancia.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Ambulancia not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [ambulanciaInstance: ambulanciaInstance]
        }
    }

    def edit = {
        def ambulanciaInstance = Ambulancia.get(params.id)
        if (!ambulanciaInstance) {
            flash.message = "ambulancia.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Ambulancia not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [ambulanciaInstance: ambulanciaInstance]
        }
    }

    def update = {
        def ambulanciaInstance = Ambulancia.get(params.id)
        if (ambulanciaInstance) {
            if (params.version) {
                def version = params.version.toLong()
                if (ambulanciaInstance.version > version) {
                    
                    ambulanciaInstance.errors.rejectValue("version", "ambulancia.optimistic.locking.failure", "Another user has updated this Ambulancia while you were editing")
                    render(view: "edit", model: [ambulanciaInstance: ambulanciaInstance])
                    return
                }
            }
            ambulanciaInstance.properties = params
            if (!ambulanciaInstance.hasErrors() && ambulanciaInstance.save()) {
                flash.message = "ambulancia.updated"
                flash.args = [params.id]
                flash.defaultMessage = "Ambulancia ${params.id} updated"
                redirect(action: "show", id: ambulanciaInstance.id)
            }
            else {
                render(view: "edit", model: [ambulanciaInstance: ambulanciaInstance])
            }
        }
        else {
            flash.message = "ambulancia.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Ambulancia not found with id ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def delete = {
        def ambulanciaInstance = Ambulancia.get(params.id)
        if (ambulanciaInstance) {
            try {
                ambulanciaInstance.delete()
                flash.message = "ambulancia.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Ambulancia ${params.id} deleted"
                redirect(action: "list")
            }
            catch (org.springframework.dao.DataIntegrityViolationException e) {
                flash.message = "ambulancia.not.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Ambulancia ${params.id} could not be deleted"
                redirect(action: "show", id: params.id)
            }
        }
        else {
            flash.message = "ambulancia.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Ambulancia not found with id ${params.id}"
            redirect(action: "list")
        }
    }
}
