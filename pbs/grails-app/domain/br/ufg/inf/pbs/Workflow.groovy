package br.ufg.inf.pbs

class Workflow {

    static constraints = {
        nome(blank: false)
    }

    static hasMany = [todasAtividades: Atividade]
    static belongsTo = Projeto
    static transients = [ 'atividades' ]

    String nome
    Projeto projeto

    List todasAtividades = []

    void adicioneAtividade(Atividade atividade) {
        todasAtividades.add(atividade)
        atividade.workflow = this
    }

    public List getAtividades() {
        todasAtividades.findAll { it.pai == null }
    }    
}
