package br.gov.go.saude.hugo.ambulancia

import grails.test.GrailsUnitTestCase

class PacienteServiceTests extends GrailsUnitTestCase {
    PacienteService pacienteService

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

    void testeObtenhaPacienteBasico() {
        Viagem viagem = new Viagem(ambulancia: ambulancia, motorista: motorista)
        viagem.paradas = [
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "João de Deus"),
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "Fulano de Oliveira"),
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "Fabio Santana"),
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "Gustavo Sousa")
        ]
        viagem.registreSaida(operador, new Date(), 12453)
        viagem.registreRetorno(operador, new Date(), 12454)
        viagem.save()
        assertEquals 1, Viagem.count()

        List pacientes = pacienteService.obtenhaPacientes()
        assertEquals 4, pacientes.size()

        assertTrue pacientes.contains('FULANO DE OLIVEIRA')
        assertTrue pacientes.contains('GUSTAVO SOUSA')
        assertEquals 'FABIO SANTANA', pacientes.first()
        assertEquals 'JOÃO DE DEUS', pacientes.last()
    }

    void testeObtenhaPacientesRepetidos() {
        Viagem viagem = new Viagem(ambulancia: ambulancia, motorista: motorista)
        viagem.paradas = [
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "João de Deus"),
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "Fulano de Oliveira"),
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "Fabio Santana"),
                new ParadaPaciente(destino: 'Hemocentro', paciente: "João de Deus")
        ]
        viagem.registreSaida(operador, new Date(), 12453)
        viagem.registreRetorno(operador, new Date(), 12454)
        viagem.save()
        assertEquals 1, Viagem.count()

        List pacientes = pacienteService.obtenhaPacientes()
        assertEquals 3, pacientes.size()

        assertTrue pacientes.contains('FULANO DE OLIVEIRA')
        assertEquals 'FABIO SANTANA', pacientes.first()
        assertEquals 'JOÃO DE DEUS', pacientes.last()
    }
}

