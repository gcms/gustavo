package br.gov.go.saude.hugo.ambulancia

import grails.test.GrailsUnitTestCase

class GerenciadorGrupoServiceTests extends GrailsUnitTestCase {
    GerenciadorGrupoService gerenciadorGrupoService

    protected void setUp() {
        super.setUp()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeRegistreGrupo() {
        assertNull Grupo.findByAuthority('ROLE_NOVO_GRUPO')
        
        gerenciadorGrupoService.registreGrupo('ROLE_NOVO_GRUPO')
        assertNotNull Grupo.findByAuthority('ROLE_NOVO_GRUPO')
    }

    void testeRegistreUsuario() {
        assertNull Operador.findByUsuario('teste')

        gerenciadorGrupoService.registreUsuario('teste', 'teste', 'ROLE_NOVO_GRUPO2')
        assertNotNull Grupo.findByAuthority('ROLE_NOVO_GRUPO2')
        assertNotNull Operador.findByUsuario('teste')
    }

    void testeGrupoUsuarioPadrao() {
        assertTrue(Operador.count() >= 1)
        assertTrue(Grupo.count() >= 2)
    }
}
