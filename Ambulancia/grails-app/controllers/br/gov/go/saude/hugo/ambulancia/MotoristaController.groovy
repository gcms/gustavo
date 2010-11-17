package br.gov.go.saude.hugo.ambulancia

class MotoristaController {

    def index = { redirect(action: "list", params: params) }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [save: "POST", update: "POST", delete: "POST"]

    def list = {
        params.max = Math.min(params.max ? params.max.toInteger() : 10,  100)
        [motoristaInstanceList: Motorista.list(params), motoristaInstanceTotal: Motorista.count()]
    }

    def create = {
        def motoristaInstance = new Motorista()
        motoristaInstance.properties = params
        return [motoristaInstance: motoristaInstance]
    }

    def save = {
        def motoristaInstance = new Motorista(params)
        if (!motoristaInstance.hasErrors() && motoristaInstance.save()) {
            flash.message = "motorista.created"
            flash.args = [motoristaInstance.id]
            flash.defaultMessage = "Motorista ${motoristaInstance.id} created"
            redirect(action: "show", id: motoristaInstance.id)
        }
        else {
            render(view: "create", model: [motoristaInstance: motoristaInstance])
        }
    }

    def show = {
        def motoristaInstance = Motorista.get(params.id)
        if (!motoristaInstance) {
            flash.message = "motorista.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Motorista not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [motoristaInstance: motoristaInstance]
        }
    }

    def edit = {
        def motoristaInstance = Motorista.get(params.id)
        if (!motoristaInstance) {
            flash.message = "motorista.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Motorista not found with id ${params.id}"
            redirect(action: "list")
        }
        else {
            return [motoristaInstance: motoristaInstance]
        }
    }

    def update = {
        def motoristaInstance = Motorista.get(params.id)
        if (motoristaInstance) {
            if (params.version) {
                def version = params.version.toLong()
                if (motoristaInstance.version > version) {
                    
                    motoristaInstance.errors.rejectValue("version", "motorista.optimistic.locking.failure", "Another user has updated this Motorista while you were editing")
                    render(view: "edit", model: [motoristaInstance: motoristaInstance])
                    return
                }
            }
            motoristaInstance.properties = params
            if (!motoristaInstance.hasErrors() && motoristaInstance.save()) {
                flash.message = "motorista.updated"
                flash.args = [params.id]
                flash.defaultMessage = "Motorista ${params.id} updated"
                redirect(action: "show", id: motoristaInstance.id)
            }
            else {
                render(view: "edit", model: [motoristaInstance: motoristaInstance])
            }
        }
        else {
            flash.message = "motorista.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Motorista not found with id ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def delete = {
        def motoristaInstance = Motorista.get(params.id)
        if (motoristaInstance) {
            try {
                motoristaInstance.delete()
                flash.message = "motorista.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Motorista ${params.id} deleted"
                redirect(action: "list")
            }
            catch (org.springframework.dao.DataIntegrityViolationException e) {
                flash.message = "motorista.not.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Motorista ${params.id} could not be deleted"
                redirect(action: "show", id: params.id)
            }
        }
        else {
            flash.message = "motorista.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Motorista not found with id ${params.id}"
            redirect(action: "list")
        }
    }
}
