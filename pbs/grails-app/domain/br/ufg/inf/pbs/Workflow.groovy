package br.ufg.inf.pbs

class Workflow {

    static constraints = {
        nome(blank: false)
    }

    static hasMany = [atividades: Atividade]
    static belongsTo = Projeto
    static transients = [ 'todasAtividades' ]

    String nome
    Projeto projeto

    List atividades = []

    void adicioneAtividade(Atividade atividade) {
        atividades.add(atividade)
    }

    public List getTodasAtividades() {
        atividades.collect { [it] + it.todasSubAtividades }.flatten()
    }
}
