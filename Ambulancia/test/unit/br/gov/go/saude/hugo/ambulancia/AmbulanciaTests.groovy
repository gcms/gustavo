package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class AmbulanciaTests extends GrailsUnitTestCase {
    protected void setUp() {
        super.setUp()
        mockDomain(Ambulancia)
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testValidacaoPlaca() {
        assertFalse new Ambulancia(placa: "NFW1234").validate()
        assertFalse new Ambulancia(placa: "NF1234").validate()
        assertFalse new Ambulancia(placa: "NFW-34").validate()
        assertFalse new Ambulancia(placa: "NFW-34  ").validate()
        assertFalse new Ambulancia(placa: "123-3445").validate()
        assertFalse new Ambulancia(placa: "NFW-JHFG").validate()

        assertTrue new Ambulancia(placa: "NFW-1178").validate()
        assertTrue new Ambulancia(placa: "JGH-5414").validate()
        assertTrue new Ambulancia(placa: "nfw-1178").validate()
    }

    void testePlacaDuplicada() {
        new Ambulancia(placa: "NFW-1178").save()
        assertEquals 1, Ambulancia.count()

        Ambulancia duplicada = new Ambulancia(placa: "NFW-1178")
        assertFalse duplicada.validate()

        duplicada.save() // item duplicado => não será salvo
        assertEquals 1, Ambulancia.count() // continua 1 após salvar
    }

    void testeSalvarOk() {
        new Ambulancia(placa: "NFW-1178").save()
        new Ambulancia(placa: "JGH-5414").save()
        new Ambulancia(placa: "nfw-1123").save()

        assertEquals 3, Ambulancia.count()
    }
}
