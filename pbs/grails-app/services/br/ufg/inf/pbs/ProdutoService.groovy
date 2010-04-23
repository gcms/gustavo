package br.ufg.inf.pbs

class ProdutoService {

    boolean transactional = true

    List obtenhaProdutosParaAtualizar(Atividade atividade) {
        assert atividade.versao == null

        List produtos = Produto.list()

        List subAtividades = atividade.todasSubAtividades
        produtos.findAll {Produto produto ->
            produto.versaoInicial != null && !subAtividades.contains(produto.versaoInicial.atividade)
        }
    }

    List obtenhaProdutosPai(Atividade atividade) {
        assert atividade.versao == null

        List produtos = Produto.list()
        List subAtividades = atividade.todasSubAtividades
        produtos.findAll {Produto produto ->
            !subAtividades.contains(produto.versaoInicial.atividade)
        }.sort { a, b -> DistanciaAtividade.calculeDistanciaAtividades(a.ultimaVersao.atividade, b.ultimaVersao.atividade) }
    }

    boolean incluaNovoProduto(Atividade atividade, Produto pai, Produto filho) {
        if (pai) {
            pai.adicioneProdutoFilho(filho)
            pai.save()
        } else {
            filho.save()
        }

        atividade.incluirVersao(filho)
        atividade.save(flush: true)
    }

    boolean incluaNovaVersaoProduto(Atividade atividade, Produto produto) {
        atividade.incluirVersao(produto)
        atividade.save(flush: true)
    }
}
