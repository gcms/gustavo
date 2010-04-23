package br.ufg.inf.pbs

class Projeto {

    static constraints = {
        nome(blank: false)
    }

    static hasMany = [workflows: Workflow, todosProdutos: Produto]
    static transients = ['produtos']

    String nome
    List workflows = []
    List todosProdutos = []

    void adicioneWorkflow(Workflow workflow) {
        workflows.add(workflow)
        workflow.projeto = this
    }

    void adicioneProduto(Produto produto) {
        todosProdutos.add(produto)
        produto.projeto = this
    }

    public List getProdutos() {
        todosProdutos.findAll { it.pai == null }
    }
}
