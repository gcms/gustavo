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
        assertFalse new Ambulancia(placa: "NFW1234", prefixo: '231').validate()
        assertFalse new Ambulancia(placa: "NF1234", prefixo: '901').validate()
        assertFalse new Ambulancia(placa: "NFW-34", prefixo: '901').validate()
        assertFalse new Ambulancia(placa: "NFW-34  ", prefixo: '901').validate()
        assertFalse new Ambulancia(placa: "123-3445", prefixo: '901').validate()
        assertFalse new Ambulancia(placa: "NFW-JHFG", prefixo: '901').validate()

        assertTrue new Ambulancia(placa: "NFW-1178", prefixo: '901').validate()
        assertTrue new Ambulancia(placa: "JGH-5414", prefixo: '901').validate()
        assertTrue new Ambulancia(placa: "nfw-1178", prefixo: '901').validate()
    }

    void testePlacaDuplicada() {
        new Ambulancia(placa: "NFW-1178", prefixo: '190').save()
        assertEquals 1, Ambulancia.count()

        Ambulancia duplicada = new Ambulancia(placa: "NFW-1178", prefixo: '190')
        assertFalse duplicada.validate()
        assertEquals 'unique', duplicada.errors['placa']

        duplicada.save() // item duplicado => não será salvo
        assertEquals 1, Ambulancia.count() // continua 1 após salvar
    }

    void testValidacaoPrefixo() {
        assertFalse new Ambulancia(placa: "NFW-1234", prefixo: 'asb').validate()
        assertFalse new Ambulancia(placa: "NFW-1234", prefixo: 'AXZ').validate()
        assertFalse new Ambulancia(placa: "NFW-1234", prefixo: '1009').validate()
        assertFalse new Ambulancia(placa: "NFW-1234", prefixo: '1ab').validate()
        assertFalse new Ambulancia(placa: "NFW-1234", prefixo: '11b').validate()
        assertFalse new Ambulancia(placa: "NFW-1234", prefixo: '1b1').validate()

        assertTrue new Ambulancia(placa: "NFW-1178", prefixo: '901').validate()
        assertTrue new Ambulancia(placa: "JGH-5414", prefixo: '10').validate()
        assertTrue new Ambulancia(placa: "nfw-1178", prefixo: '3').validate()
    }

    void testeSalvarOk() {
        new Ambulancia(placa: "NFW-1178", prefixo: '109').save()
        new Ambulancia(placa: "JGH-5414", prefixo: '902').save()
        new Ambulancia(placa: "nfw-1123", prefixo: '903').save()

        assertEquals 3, Ambulancia.count()
    }
}
