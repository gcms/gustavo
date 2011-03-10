package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class MotoristaServiceTests extends GrailsUnitTestCase {
    GerenciamentoGrupoService gerenciamentoGrupoService
    GerenciamentoViagemService gerenciamentoViagemService
    MotoristaService motoristaService

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

    void testeUmMotoristaOcupado() {
        Viagem viagem = new Viagem(ambulancia: ambulancia1, motorista: motorista1)
        viagem.registreSaida(operador, new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        assertEquals 2, motoristaService.obtenhaMotoristasDisponiveis().size()
    }

    void testeMotoristasOcupados() {
        Viagem viagem = new Viagem(ambulancia: ambulancia1, motorista: motorista1)
        viagem.registreSaida(operador, new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        assertEquals 2, motoristaService.obtenhaMotoristasDisponiveis().size()

        viagem = new Viagem(ambulancia: ambulancia2, motorista: motorista2)
        viagem.registreSaida(operador, new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 2, Viagem.count()

        assertEquals 1, motoristaService.obtenhaMotoristasDisponiveis().size()

        viagem = new Viagem(ambulancia: ambulancia3, motorista: motorista3)
        viagem.registreSaida(operador, new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 3, Viagem.count()

        assertEquals 0, motoristaService.obtenhaMotoristasDisponiveis().size()
    }

    void testeMotoristaDesativado() {
        assertEquals 3, motoristaService.obtenhaMotoristasDisponiveis().size()

        motorista1.disponivel = false
        motorista1.save()

        assertEquals 2, motoristaService.obtenhaMotoristasDisponiveis().size()
    }
}
