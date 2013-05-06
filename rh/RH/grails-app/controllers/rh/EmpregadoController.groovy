package rh

class EmpregadoController {

    def index() { }

    def novo = {
        render view: 'edit', model: [pessoa: new Pessoa()]
    }

    def salve = {
        println params
        redirect action: novo
    }
}
