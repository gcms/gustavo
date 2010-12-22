package br.gov.go.saude.hugo.ambulancia

import grails.test.GrailsUnitTestCase

class GerenciamentoGrupoServiceTests extends GrailsUnitTestCase {
    GerenciamentoGrupoService gerenciamentoGrupoService

    protected void setUp() {
        super.setUp()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeRegistreGrupo() {
        assertNull Grupo.findByAuthority('ROLE_NOVO_GRUPO')
        
        gerenciamentoGrupoService.registreGrupo('ROLE_NOVO_GRUPO')
        assertNotNull Grupo.findByAuthority('ROLE_NOVO_GRUPO')
    }

    void testeRegistreUsuario() {
        assertNull Operador.findByUsuario('teste')

        gerenciamentoGrupoService.registreUsuario('teste', 'teste', 'ROLE_NOVO_GRUPO2')
        assertNotNull Grupo.findByAuthority('ROLE_NOVO_GRUPO2')
        assertNotNull Operador.findByUsuario('teste')

        Operador teste = Operador.findByUsuario('teste')
        assertNotNull teste
        assertEquals 'teste', teste.usuario
        assertEquals 'Teste', teste.nome

        assertEquals 1, OperadorGrupo.findAllByOperador(teste).size()
    }

    void testeGrupoUsuarioPadrao() {
        assertTrue(Operador.count() >= 1)
        assertTrue(Grupo.count() >= 2)
    }

    void testeRegistroSenhaNaoBate() {
        Operador operador = new Operador(usuario: 'teste', enabled: true)
        assertFalse gerenciamentoGrupoService.crieOperador(operador, 'teste', 'teste1')
    }

    void testeRegistroSenhaOk() {
        Operador operador = new Operador(usuario: 'teste', enabled: true)
        assertTrue gerenciamentoGrupoService.crieOperador(operador, 'teste', 'teste')

        assertEquals 1, OperadorGrupo.findAllByOperador(operador).size()
    }

    void testeAtualizaSenhaNaoBate() {
        Operador operador = new Operador(usuario: 'teste', enabled: true)
        assertTrue gerenciamentoGrupoService.crieOperador(operador, 'teste', 'teste')

        operador = Operador.list().first()
        assertFalse gerenciamentoGrupoService.salveOperador(operador, 'teste', 'teste1')
        assertEquals 'operador.senha.matches.invalid', operador.errors.getFieldError('senha').code
    }

    void testeRegistroUsuarioDuplicado() {
        Operador operador1 = new Operador(usuario: 'teste', enabled: true)
        assertTrue gerenciamentoGrupoService.crieOperador(operador1, 'teste', 'teste')
        assertNotNull Operador.findByUsuario('teste')

        Operador operador2 = new Operador(usuario: 'teste', enabled: true)
        assertFalse gerenciamentoGrupoService.crieOperador(operador2, 'teste', 'teste')

        assertEquals 1, Operador.executeQuery("from Operador o where o.usuario = 'teste'").size()
        assertEquals 1, OperadorGrupo.executeQuery("from OperadorGrupo og where og.operador.usuario = 'teste'").size()
    }
}
