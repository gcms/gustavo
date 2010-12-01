package br.gov.go.saude.hugo.ambulancia

import grails.test.GrailsUnitTestCase

class AmbulanciaServiceTests extends GrailsUnitTestCase {
    GerenciamentoGrupoService gerenciamentoGrupoService
    GerenciamentoViagemService gerenciamentoViagemService
    AmbulanciaService ambulanciaService

    Motorista motorista1
    Motorista motorista2
    Motorista motorista3

    Ambulancia ambulancia1
    Ambulancia ambulancia2
    Ambulancia ambulancia3

    Operador operador

    protected void setUp() {
        super.setUp()

        motorista1 = new Motorista(nome: 'motorista1')
        motorista1.save()

        motorista2 = new Motorista(nome: 'motorista2')
        motorista2.save()

        motorista3 = new Motorista(nome: 'motorista3')
        motorista3.save()

        ambulancia1 = new Ambulancia(placa: 'asd-1234', prefixo: '901')
        ambulancia1.save()

        ambulancia2 = new Ambulancia(placa: 'asd-1233', prefixo: '902')
        ambulancia2.save()

        ambulancia3 = new Ambulancia(placa: 'asd-1232', prefixo: '903')
        ambulancia3.save()

        operador = gerenciamentoGrupoService.registreUsuario('fulano', 'teste', 'ROLE_USER') 
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeUmaAmbulanciaOcupada() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia1, motorista: motorista1)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        assertEquals 2, ambulanciaService.obtenhaAmbulanciasDisponiveis().size()
    }

    void testeAmbulanciasOcupadas() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia1, motorista: motorista1)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        assertEquals 2, ambulanciaService.obtenhaAmbulanciasDisponiveis().size()

        viagem = new Viagem(operador: operador, ambulancia: ambulancia2, motorista: motorista2)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 2, Viagem.count()

        assertEquals 1, ambulanciaService.obtenhaAmbulanciasDisponiveis().size()

        viagem = new Viagem(operador: operador, ambulancia: ambulancia3, motorista: motorista3)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 3, Viagem.count()

        assertEquals 0, ambulanciaService.obtenhaAmbulanciasDisponiveis().size()
    }

    void testeAmbulanciaIndisponivel() {
        assertEquals 3, ambulanciaService.obtenhaAmbulanciasDisponiveis().size()

        ambulancia2.disponivel = false

        assertEquals 2, ambulanciaService.obtenhaAmbulanciasDisponiveis().size()
    }

    void testeKmAmbulancia() {
        assertEquals 0, ambulanciaService.obtenhaKmRetornoUltimaViagem(ambulancia1)
        assertEquals 0, ambulanciaService.obtenhaKmRetornoUltimaViagem(ambulancia2)
        assertEquals 0, ambulanciaService.obtenhaKmRetornoUltimaViagem(ambulancia3)

        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia1, motorista: motorista1)
        viagem.registreSaida(new Date(), 0)
        assertTrue gerenciamentoViagemService.registreSaida(viagem)

        viagem.registreRetorno(new Date(), 1024)
        assertTrue gerenciamentoViagemService.registreRetorno(viagem)

        assertEquals 1024, ambulanciaService.obtenhaKmRetornoUltimaViagem(ambulancia1)
    }
}
