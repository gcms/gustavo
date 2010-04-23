package br.ufg.inf.pbs

import grails.test.GrailsUnitTestCase
import org.hibernate.SessionFactory

class AtividadeTests extends GrailsUnitTestCase {
    SessionFactory sessionFactory

    protected void setUp() {
        super.setUp()
    }

    protected void tearDown() {
        super.tearDown()
    }


    void testeCriacaoLeitura() {
        Projeto projeto = new Projeto(nome: 'PBS Workflow')
        assertNotNull projeto.save(flush: true)

        Workflow workflow = new Workflow(nome: 'Desenvolvimento')
        projeto.adicioneWorkflow(workflow)

        workflow.adicioneAtividade(new Atividade(nome: 'Requisitos'))
        workflow.adicioneAtividade(new Atividade(nome: 'Desenvolvimento'))
        workflow.adicioneAtividade(new Atividade(nome: 'Testes'))

        assertNotNull workflow.save(flush: true)

        sessionFactory.currentSession.clear()

        workflow = Workflow.list().first()
        assertEquals 3, workflow.todasAtividades.size()
        assertEquals 'Requisitos', workflow.todasAtividades.first().nome
    }

    void testeMultiplosNiveis() {
        Projeto projeto = new Projeto(nome: 'PBS Workflow')
        assertNotNull projeto.save(flush: true)

        Workflow workflow = new Workflow(nome: 'Desenvolvimento')
        projeto.adicioneWorkflow(workflow)

        Atividade req = new Atividade(nome: 'Requisitos')
        workflow.adicioneAtividade(req)
        req.adicioneAtividade(new Atividade(nome: 'Elaborar casos de uso'))
        req.adicioneAtividade(new Atividade(nome: 'Elaborar casos de teste'))

        workflow.adicioneAtividade(new Atividade(nome: 'Desenvolvimento'))
        workflow.adicioneAtividade(new Atividade(nome: 'Testes'))

        assertNotNull workflow.save(flush: true)

        sessionFactory.currentSession.clear()

        workflow = Workflow.list().first()
        assertEquals 3, workflow.atividades.size()
        Atividade at = workflow.todasAtividades.first()
        assertEquals 'Requisitos', at.nome
        assertEquals 2, at.subAtividades.size()
    }
}
