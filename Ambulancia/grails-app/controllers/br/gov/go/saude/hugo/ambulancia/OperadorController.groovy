package br.gov.go.saude.hugo.ambulancia

class OperadorController {

    def index = { redirect(action: "list", params: params) }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [save: "POST", update: "POST", delete: "POST"]

    def list = {
        params.max = Math.min(params.max ? params.max.toInteger() : 10,  100)
        [operadorInstanceList: Operador.list(params), operadorInstanceTotal: Operador.count()]
    }

    def create = {
        def operadorInstance = new Operador()
        operadorInstance.properties = params
        return [operadorInstance: operadorInstance]
    }

    def save = {
        def operadorInstance = new Operador(params)
        if (!operadorInstance.hasErrors() && operadorInstance.save()) {
            flash.message = "operador.created"
            flash.args = [operadorInstance.id]
            flash.defaultMessage = "Operador ${operadorInstance.id} created"
            redirect(action: "show", id: operadorInstance.id)
        }
        else {
            render(view: "create", model: [operadorInstance: operadorInstance])
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
            if (params.version) {
                def version = params.version.toLong()
                if (operadorInstance.version > version) {
                    
                    operadorInstance.errors.rejectValue("version", "operador.optimistic.locking.failure", "Another user has updated this Operador while you were editing")
                    render(view: "edit", model: [operadorInstance: operadorInstance])
                    return
                }
            }
            operadorInstance.properties = params
            if (!operadorInstance.hasErrors() && operadorInstance.save()) {
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
                operadorInstance.delete()
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
