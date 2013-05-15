package rh

import grails.converters.JSON
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
        println empregado.dump()

        if (!empregado.pessoa.hasErrors() && empregado.pessoa.save() && !empregado.hasErrors() && empregado.save()) {
            flash.message = 'Empregado salvo com sucesso'
            redirect action: index
        } else {
            flash.message = 'Erro!'
            render(view: 'edit', model: [empregado: empregado], params: params)
        }
    }

    static private loadEmpregado(TypeConvertingMap params) {
        Empregado empregado = Empregado.get(params.id)
        empregado.properties = params.empregado
        empregado
    }

    def pessoas = {
        List pessoas = Pessoa.executeQuery("from Pessoa where upper(nome) like upper(:nome || '%')", [nome: params.term])
        pessoas = pessoas.collect { it.properties + [label: it.nome]}
        render(pessoas as JSON)
    }
}
