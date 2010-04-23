
package br.ufg.inf.pbs

class Atividade {

    static constraints = {
        nome(blank: false)
        pai(nullable: true)
        versao(nullable: true)
    }

    static hasMany = [filhos: Atividade]
    static belongsTo = [Workflow, Atividade]

    static transients = [ 'subAtividades', 'todasSubAtividades', 'projeto' ]

    String nome

    Workflow workflow
    Atividade pai
    List filhos = []

    Versao versao

    List getSubAtividades() {
        filhos
    }

    List getTodasSubAtividades() {
        subAtividades.collect { [it] + it.todasSubAtividades }.flatten()
    }

    void adicioneAtividade(Atividade atividade) {
        filhos.add(atividade)
        atividade.pai = this
        workflow.adicioneAtividade(atividade)
    }

    void incluirVersao(Produto produto) {
        assert versao == null
        versao = produto.novaVersao(this)
    }

    void definaWorkflow(Workflow workflow) {
        this.workflow = workflow
        filhos.each { it.definaWorkflow(workflow) }
    }

    Projeto getProjeto() {
        workflow.projeto
    }
}
