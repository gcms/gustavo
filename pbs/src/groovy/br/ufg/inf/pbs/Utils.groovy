package br.ufg.inf.pbs

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 23/04/2010
 * Time: 13:48:34
 * To change this template use File | Settings | File Templates.
 */
class Utils {
    static Projeto obtenhaProdutoInicializado() {
        Projeto projeto = new Projeto(nome: 'PBS')

        Workflow workflow = new Workflow(nome: 'Desenvolvimento')
        projeto.adicioneWorkflow(workflow)

        Atividade req = new Atividade(nome: 'Requisitos')
        workflow.adicioneAtividade(req)
        req.adicioneAtividade(new Atividade(nome: 'Elaborar casos de uso'))
        req.adicioneAtividade(new Atividade(nome: 'Elaborar casos de teste'))
        workflow.adicioneAtividade(new Atividade(nome: 'Desenvolvimento'))

        Atividade tes = new Atividade(nome: 'Testes')
        workflow.adicioneAtividade(tes)
        tes.adicioneAtividade(new Atividade(nome: 'Executar casos de teste'))        

        projeto
    }
}
