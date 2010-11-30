package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class ParadaTests extends GrailsUnitTestCase {
    protected void setUp() {
        super.setUp()

//        mockDomain(Parada)
        mockDomain(ParadaPaciente)
        mockDomain(ParadaServicos)
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeBasico() {
        assertFalse new ParadaPaciente(destino: 'Hospital XYZ').validate()
        assertTrue new ParadaPaciente(destino: 'Hospital XYZ', paciente: 'João da Silva').validate()

        assertFalse new ParadaServicos(destino: 'Hospital XYZ').validate()
        assertTrue new ParadaServicos(destino: 'Hospital XYZ', descricao: 'Fui fazer xyz').validate()
    }
}
