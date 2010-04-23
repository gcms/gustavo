package br.ufg.inf.pbs

class Projeto {

    static constraints = {
        nome(blank: false)
    }

    static hasMany = [workflows: Workflow]

    String nome
    List workflows = []

    void adicioneWorkflow(Workflow workflow) {
        workflows.add(workflow)
        workflow.projeto = this
    }
}
