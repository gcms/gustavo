package br.ufg.inf.pbs

import grails.test.*
import org.hibernate.SessionFactory

class ProdutoTests extends GrailsUnitTestCase {
    SessionFactory sessionFactory
    protected void setUp() {
        super.setUp()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testCadastrarNovoProduto() {
        Projeto projeto = Utils.obtenhaProdutoInicializado()
        projeto.save(flush: true)

        Workflow wf = projeto.workflows.first()
        assertEquals 3, wf.atividades.size()
        assertEquals 6, wf.todasAtividades.size()
        Atividade requisitos = wf.todasAtividades.find { it.nome == 'Requisitos'}

        //
        Produto produto = new Produto(nome: 'Especificação de Requisitos de Software')
        projeto.adicioneProduto(produto)
        produto.save(flush: true)

        requisitos.incluirVersao(produto)
        requisitos.save(flush: true)
        //

        sessionFactory.currentSession.clear()

        assertEquals 1, Produto.list().size()
        Produto ers = Produto.list().first()

        assertNull ers.pai
    }

    void testCadastrarNovoProdutoComPai() {
        Projeto projeto = Utils.obtenhaProdutoInicializado()
        projeto.save(flush: true)

        Workflow wf = projeto.workflows.first()
        //
        Produto ers = new Produto(nome: 'Especificação de Requisitos de Software', projeto: projeto)
        projeto.adicioneProduto(ers)
        ers.save(flush: true)

        Atividade requisitos = wf.todasAtividades.find { it.nome == 'Requisitos'}
        requisitos.incluirVersao(ers)
        requisitos.save(flush: true)
        //
        Produto ct = new Produto(nome: 'Casos de Teste')
        ers.adicioneProdutoFilho(ct)
        ers.save(flush: true)

        Atividade especTeste = wf.todasAtividades.find { it.nome == 'Elaborar casos de uso'}
        especTeste.incluirVersao(ct)
        especTeste.save(flush: true)
        //

        sessionFactory.currentSession.clear()
        assertEquals 2, Produto.list().size()
        assertEquals 2, Versao.list().size()

        ers = Produto.list().find { it.nome.startsWith('Espec')}
        ct = Produto.list().find { it.nome.startsWith('Casos')}

        assertNotNull ct.pai
        assertEquals ers, ct.pai

        assertEquals 1, ers.filhos.size()
    }
}
