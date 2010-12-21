package br.gov.go.saude.hugo.ambulancia

import grails.test.*

class DestinoServiceTests extends GrailsUnitTestCase {
    DestinoService destinoService

    Operador operador
    Motorista motorista
    Ambulancia ambulancia

    protected void setUp() {
        super.setUp()

        operador = new Operador(usuario: 'gustavocms', senha: 'hello')
        operador.save()

        motorista = new Motorista(nome: 'Fulano da Silva')
        motorista.save()

        ambulancia = new Ambulancia(placa: 'NFS-1234', prefixo: '901')
        ambulancia.save()
    }

    protected void tearDown() {
        super.tearDown()
    }

    void testeObtenhaDestinosBasico() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.paradas = [
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "João de Deus"),
                new ParadaServicos(destino: 'Hemocentro', descricao: 'Coleta de sangue')
        ]
        viagem.registreSaida(new Date(), 12453)
        viagem.registreRetorno(new Date(), 12454)
        viagem.save()
        assertEquals 1, Viagem.count()

        List destinos = destinoService.obtenhaDestinos()
        assertEquals 2, destinos.size()

        assertTrue destinos.contains('HEMOCENTRO')
        assertTrue destinos.contains('HOSPITAL SANTA LÚCIA')
    }

    void testeObtenhaDestinosRepetidos() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.paradas = [
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "João de Deus"),
                new ParadaServicos(destino: 'Hemocentro', descricao: 'Coleta de sangue'),
                new ParadaServicos(destino: 'Hemocentro', descricao: 'Entrega de sangue')
        ]
        viagem.registreSaida(new Date(), 12453)
        viagem.registreRetorno(new Date(), 12454)
        viagem.save()
        assertEquals 1, Viagem.count()

        List destinos = destinoService.obtenhaDestinos()
        assertEquals 2, destinos.size()

        assertTrue destinos.contains('HEMOCENTRO')
        assertTrue destinos.contains('HOSPITAL SANTA LÚCIA')
    }
}
