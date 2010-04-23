package br.ufg.inf.pbs

import grails.test.*
import org.hibernate.SessionFactory

class WorkflowTests extends GrailsUnitTestCase {
    SessionFactory sessionFactory

    protected void setUp() {
        super.setUp()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeCriacaoSemProjeto() {
        Workflow workflow = new Workflow(nome: 'Desenvolvimento')

        assertNull workflow.save(flush: true)
    }

    void testeCriacao() {
        Projeto projeto = new Projeto(nome: 'PBS Workflow')
        assertNotNull projeto.save(flush: true)

        Workflow workflow = new Workflow(nome: 'Desenvolvimento')
        projeto.adicioneWorkflow(workflow)

        assertNotNull workflow.save(flush: true)
    }

    void testeCriacaoLeitura() {
        Projeto projeto = new Projeto(nome: 'PBS Workflow')
        projeto.adicioneWorkflow(new Workflow(nome: 'Desenvolvimento'))
        projeto.adicioneWorkflow(new Workflow(nome: 'Manutenção'))

        assertNotNull projeto.save(flush: true)
        sessionFactory.currentSession.clear()

        projeto = Projeto.list().first()
        assertEquals 2, projeto.workflows.size()
    }

    void testePropriedadesTransient() {
        Projeto projeto = Utils.obtenhaProdutoInicializado()
        projeto.save(flush: true)

        Workflow wf = projeto.workflows.first()
        assertEquals 3, wf.atividades.size()
        assertEquals 6, wf.todasAtividades.size()
    }
}