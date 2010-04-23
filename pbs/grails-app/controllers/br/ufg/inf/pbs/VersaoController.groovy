package br.ufg.inf.pbs

class VersaoController {

    def view = {
        [versao: Versao.get(params.id)]
    }
}
