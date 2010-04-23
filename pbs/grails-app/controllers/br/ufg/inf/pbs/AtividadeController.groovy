package br.ufg.inf.pbs

class AtividadeController {
    ProdutoService produtoService

    static layout = 'atividade'    

    def view = {
        [atividade: Atividade.get(params.id)]
    }

    def gerar = {
        Atividade atividade = Atividade.get(params.id)
        [atividade: atividade, produtos: produtoService.obtenhaProdutosPai(atividade)]
    }

    def atualizar = {
        Atividade atividade = Atividade.get(params.id)
        [atividade: atividade, produtos: produtoService.obtenhaProdutosParaAtualizar(atividade)]
    }

    def salvarProdutoVersao = {
        Atividade atividade = Atividade.get(params.id)

        Produto pai = Produto.get(params.pai)
        Produto produto = new Produto(nome: params.nome)
        produtoService.incluaNovoProduto(atividade, pai, produto)

        redirect(action: view, id: atividade.id)
    }

    def atualizarProdutoVersao = {
        Atividade atividade = Atividade.get(params.id)

        Produto produto = Produto.get(params.produto)
        produtoService.incluaNovaVersaoProduto(atividade, produto)

        redirect(action: view, id: atividade.id)
    }
}
