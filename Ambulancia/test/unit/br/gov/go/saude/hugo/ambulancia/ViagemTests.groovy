package br.gov.go.saude.hugo.ambulancia

import grails.test.*
import java.text.SimpleDateFormat
import static java.util.Calendar.*

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
        mockDomain(ParadaPaciente)
        mockDomain(ParadaServicos)

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

    void testeParadas() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(new Date(), 12453)
        viagem.paradas = [
                new ParadaPaciente(destino: 'Hospital Santa Lúcia', paciente: "João de Deus"),
                new ParadaServicos(destino: 'Hemocentro', descricao: 'Coleta de sangue')
        ]
        viagem.save()
        assertEquals 1, Viagem.count()

        viagem = Viagem.list().first()
        assertEquals 2, viagem.paradas.size()
    }

    void testeDataHora() {
        Viagem viagem = new Viagem(operador: operador, ambulancia: ambulancia, motorista: motorista)

        Calendar diaHora = Calendar.instance
        diaHora.time = new SimpleDateFormat('dd/MM/yyyy HH:mm').parse('12/12/2010 09:31')
        Calendar horaSaida = Calendar.instance
        Calendar dataSaida = Calendar.instance


        viagem.dataSaida = diaHora.time
        dataSaida.time = viagem.dataSaida

        assertEquals 2010, dataSaida[YEAR]
        assertEquals 11, dataSaida[MONTH]
        assertEquals 12, dataSaida[DAY_OF_MONTH]


        viagem.horaSaida = diaHora.time
        horaSaida.time = viagem.horaSaida
        dataSaida.time = viagem.dataSaida

        assertEquals 2010, dataSaida[YEAR]
        assertEquals 11, dataSaida[MONTH]
        assertEquals 12, dataSaida[DAY_OF_MONTH]
        assertEquals 9, horaSaida[HOUR]
        assertEquals 31, horaSaida[MINUTE]


        viagem.dataSaida = diaHora.time
        horaSaida.time = viagem.horaSaida
        dataSaida.time = viagem.dataSaida

        assertEquals 2010, dataSaida[YEAR]
        assertEquals 11, dataSaida[MONTH]
        assertEquals 12, dataSaida[DAY_OF_MONTH]
        assertEquals 9, horaSaida[HOUR]
        assertEquals 31, horaSaida[MINUTE]

    }
}
