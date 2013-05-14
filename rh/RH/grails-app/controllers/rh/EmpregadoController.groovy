package rh

import org.codehaus.groovy.grails.web.util.TypeConvertingMap

class EmpregadoController {

    def index = {
        redirect action: list
    }

    def list = {
        [empregados: Empregado.list()]
    }

    def listAtivos = {
        render view: 'list', model: []
    }

    def listInativos = {
        render view: 'list', model: []
    }

    def edit = {
        Empregado empregado = params.id ? Empregado.get(params.id) : new Empregado()

        [empregado: empregado]
    }

    def save = {
        println params
        Empregado empregado = params.id ? loadEmpregado(params) : new Empregado(params)

        if (!empregado.hasErrors() && empregado.save()) {
            flash.message = 'Empregado salvo com sucesso'
            redirect action: index
        } else {
            flash.message = 'Erro!'
            render view: edit, model: [empregado: empregado], params: params
        }
    }

    static private loadEmpregado(TypeConvertingMap params) {
        Empregado empregado = Empregado.get(params.id)
        empregado.properties = params.empregado
        empregado
    }
}
