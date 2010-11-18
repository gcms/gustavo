package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class MotoristaTests extends GrailsUnitTestCase {
    protected void setUp() {
        super.setUp()
        mockDomain(Motorista)
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testNomeValido() {
        assertFalse new Motorista().validate()

        assertTrue new Motorista(nome: "Fulano da Silva").validate()
    }

    void testMotoristaDuplicado() {
        new Motorista(nome: "Fulano da Silva").save()
        assertEquals 1, Motorista.count()

        Motorista duplicado = new Motorista(nome: "Fulano da Silva")
        assertFalse duplicado.validate()

        duplicado.save() // item duplicado => não será salvo
        assertEquals 'unique', duplicado.errors['nome']
        assertEquals 1, Motorista.count() // continua 1 item após salvar
    }

    void testeSalvarOk() {
        new Motorista(nome: "Fulano da Silva").save()
        new Motorista(nome: "Beltrano de Oliveira").save()

        assertEquals 2, Motorista.count()
    }
}
