package br.ufg.inf.pbs

class ProdutoController {

    def view = {
        [produto: Produto.get(params.id)]
    }
}
