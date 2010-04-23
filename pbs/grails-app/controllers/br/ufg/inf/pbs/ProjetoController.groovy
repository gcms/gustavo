package br.ufg.inf.pbs

class ProjetoController {

    def index = {
        redirect(action: list)
    }

    def list = {
        [projetos: Projeto.list()]
    }

    def view = {
        [projeto: Projeto.get(params.id)]
    }
}
