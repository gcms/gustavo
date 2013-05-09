package rh

class EmpregadoController {

    def index = { }

    def novo = {
        render view: 'edit', model: [empregado: new Empregado()]
    }

    def edit = {
        Empregado empregado = params.id ? Empregado.get(params.id) : new Empregado()

        render view: 'edit', model: [empregado: empregado]
    }

    def salve = {
        println params
//        Empregado empregado = params.empregado?.id ? loadEmpregado() : new Empregado()
//
//        if (!empregado.hasErrors() && empregado.save()) {
//            flash.message = 'Empregado salvo com sucesso'
//            redirect action: index
//        }
        flash.message = 'Empregado cadastrado!'
        redirect action: index
    }

    private loadEmpregado() {
        Empregado empregado = Empregado.get(params.empregado.id)
        empregado.properties = params.empregado
        empregado
    }
}
