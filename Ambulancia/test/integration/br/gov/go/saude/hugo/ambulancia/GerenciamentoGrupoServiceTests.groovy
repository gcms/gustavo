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

        Operador teste = Operador.list().first()
        assertNotNull teste

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
        assertEquals 'matches.invalid', operador.errors.getFieldError('senha').code
    }
}
