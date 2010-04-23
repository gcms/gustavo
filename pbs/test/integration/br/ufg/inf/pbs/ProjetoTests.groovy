package br.ufg.inf.pbs

import grails.test.*

class ProjetoTests extends GrailsUnitTestCase {
    protected void setUp() {
        super.setUp()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeCriacao() {
        Projeto projeto = new Projeto(nome: 'PBS Workflow')

        assertNotNull projeto.save(flush: true)
    }

    void testeCriacaoSemNome() {
        Projeto projeto = new Projeto()

        assertNull projeto.save(flush: true)
    }
}
