package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class GerenciamentoViagemServiceTests extends GrailsUnitTestCase {
    GerenciamentoGrupoService gerenciamentoGrupoService
    GerenciamentoViagemService gerenciamentoViagemService

    Operador operador
    Motorista motorista
    Ambulancia ambulancia

    protected void setUp() {
        super.setUp()

        operador = gerenciamentoGrupoService.registreUsuario('gustavocms', 'hello', 'ROLE_USER')

        motorista = new Motorista(nome: 'Fulano da Silva')
        motorista.save()

        ambulancia = new Ambulancia(placa: 'NFS-1234')
        ambulancia.save()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeSaidasSimultaneas() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        assertEquals 'unique', viagem.errors.getFieldError('ambulancia').code
        assertEquals 'unique', viagem.errors.getFieldError('motorista').code
    }

    void testeSaidasOk() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        Ambulancia ambulancia2 = new Ambulancia(placa: 'nsw-1234')
        ambulancia2.save()

        Motorista motorista2 = new Motorista(nome: "Ciclano da Silva")
        motorista2.save()

        viagem = new Viagem(operador: operador, ambulancia: ambulancia2, motorista: motorista2)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 2, Viagem.count()
    }

    void testeSaidasMsmMotorista() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        Ambulancia ambulancia2 = new Ambulancia(placa: 'nsw-1234')
        ambulancia2.save()

        viagem = new Viagem(operador: operador, ambulancia: ambulancia2, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()
        
        assertEquals 'unique', viagem.errors.getFieldError('motorista').code
    }

    void testeSaidaRetorno() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        gerenciamentoViagemService.registreSaida(viagem)
        viagem.registreRetorno(new Date(), 12456)
        gerenciamentoViagemService.registreRetorno(viagem)

        assertEquals 1, Viagem.count()

        // Retornou... pode iniciar uma nova viagem com mesma ambulancia/motorista
        viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12456)
        gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 2, Viagem.count()
    }

    void testeMsmAmbulanciaKmAnterior() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)

        viagem.registreSaida(new Date(), 1500)
        assertTrue gerenciamentoViagemService.registreSaida(viagem)
        assertEquals 1, Viagem.count()

        viagem.registreRetorno(new Date(), 3000)
        assertTrue gerenciamentoViagemService.registreRetorno(viagem)
        assertEquals 1, Viagem.count()

        viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 2000)
        assertFalse gerenciamentoViagemService.registreSaida(viagem); // km anterior ao de retorno da última viagem

        assertEquals 'min.notmet', viagem.errors.getFieldError('kmSaida').code        
    }
}
