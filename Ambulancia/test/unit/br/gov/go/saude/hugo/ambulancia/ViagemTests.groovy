package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class ViagemTests extends GrailsUnitTestCase {
    Operador operador
    Motorista motorista
    Ambulancia ambulancia

    protected void setUp() {
        super.setUp()

        mockDomain(Operador)
        mockDomain(Ambulancia)
        mockDomain(Motorista)
        mockDomain(Viagem)

        operador = new Operador(usuario: 'gustavocms', senha: 'hello')
        operador.save()

        motorista = new Motorista(nome: 'Fulano da Silva')
        motorista.save()

        ambulancia = new Ambulancia(placa: 'NFS-1234')
        ambulancia.save()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeViagemSemMotoristaAmbulancia() {
        Viagem viagem = new Viagem(operador: operador)
        viagem.registreSaida(new Date(), 12453)
        assertFalse viagem.validate()
        
        assertEquals 'nullable', viagem.errors['ambulancia']
        assertEquals 'nullable', viagem.errors['motorista']

        viagem.ambulancia = ambulancia
        assertFalse viagem.validate()

        assertEquals 'nullable', viagem.errors['motorista']

        viagem.motorista = motorista
        assertTrue viagem.validate()
    }

    void testeRetorno() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        viagem.save()
        assertEquals 1, Viagem.count()

        viagem = Viagem.list().first()
        assertFalse viagem.jaRetornou()

        viagem.registreRetorno(new Date(), 12454)
        viagem.save()

        viagem = Viagem.list().first()
        assertTrue viagem.jaRetornou()
    }

    void testeRetornoDistancia() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        viagem.save()
        assertEquals 1, Viagem.count()

        viagem = Viagem.list().first()
        assertFalse viagem.jaRetornou()

        viagem.registreRetorno(new Date(), 12464)
        viagem.save()

        viagem = Viagem.list().first()
        assertTrue viagem.jaRetornou()
        assertEquals 11, viagem.distancia
    }

    void testeRetornoIncorreto() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        viagem.registreRetorno(null, 12456)
        assertFalse viagem.validate()
    }

    void testePacientes() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        viagem.pacientes = ["João de Deus", "Maria da Silva"]
        viagem.save()
        assertEquals 1, Viagem.count()

        viagem = Viagem.list().first()
        assertEquals 2, viagem.pacientes.size()
    }
}
