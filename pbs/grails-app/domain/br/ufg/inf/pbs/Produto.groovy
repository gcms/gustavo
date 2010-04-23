package br.ufg.inf.pbs

class Produto {

    static constraints = {
        nome(blank: false)
        pai(nullable: true)
    }

    static hasMany = [versoes: Versao, filhos: Produto]
    static mappedBy = [filhos: 'pai']
    static transients = ['versaoInicial', 'ultimaVersao']
    static belongsTo = [/*Projeto,*/ Produto]

    String nome
    List versoes = []

    Produto pai
    Projeto projeto
    List filhos = []

    public Versao getVersaoInicial() {
        versoes.empty ? null : versoes.first()
    }

    public Versao getUltimaVersao() {
        versoes.empty ? null : versoes.last()
    }

    void adicioneProdutoFilho(Produto produto) {
        filhos.add(produto)
        produto.pai = this
        projeto.adicioneProduto(produto)
    }

    Versao novaVersao(Atividade atividade) {
        int numero = ultimaVersao?.numero ?: 0
        Versao versao = new Versao(numero: numero + 1, atividade: atividade, produto: this)
        versoes.add(versao)
        versao
    }
}
