package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.utilitario.UtilitarioDataHorario
import grails.test.ControllerUnitTestCase
import org.hibernate.SessionFactory
import br.gov.go.saude.hugo.utilitario.FormatadorDataHora
import grails.test.GrailsMock

class ViagemControllerTests extends ControllerUnitTestCase {
    GerenciamentoViagemService gerenciamentoViagemService
    SessionFactory sessionFactory

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

    private Viagem registreViagemComSaida(Long kmSaida) {
        Viagem viagem = new Viagem(ambulancia: ambulancia, motorista: motorista)
        viagem.registreSaida(operador, new Date(), kmSaida)
        assertTrue gerenciamentoViagemService.registreSaida(viagem)
        return viagem
    }

    private Viagem registreViagemComRetorno(Long kmSaida, Long kmRetorno) {
        Viagem viagem = registreViagemComSaida(kmSaida)
        viagem.registreRetorno(operador, new Date(), kmRetorno)
        assertTrue gerenciamentoViagemService.registreRetorno(viagem)
        assertNotNull Viagem.get(viagem.id)
        return viagem
    }

    void testeAlteracaoViagemRetornadaOk() {
        Viagem viagem = registreViagemComRetorno(120, 130)

        mockParams.id = viagem.id
        mockParams['ambulancia.id'] = viagem.ambulancia.id
        mockParams['motorista.id'] = viagem.motorista.id

        mockParams.dataSaida = FormatadorDataHora.default.dateFormat.format(viagem.dataSaida)
        mockParams.horaSaida = FormatadorDataHora.default.timeFormat.format(viagem.horaSaida)
        mockParams.kmSaida = viagem.kmSaida

        mockParams.dataRetorno = FormatadorDataHora.default.dateFormat.format(viagem.dataRetorno)
        mockParams.horaRetorno = FormatadorDataHora.default.timeFormat.format(viagem.horaRetorno)
        mockParams.kmRetorno = 125
        controller.update()

        sessionFactory.currentSession.clear()

        viagem = Viagem.get(viagem.id)
        assertEquals 125, viagem.kmRetorno
    }

    void testeAlteracaoComKmSaidaMenorKmRetorno() {
        registreViagemComRetorno(120, 130)
        Viagem viagem = registreViagemComRetorno(130, 145)

        mockParams.id = viagem.id
        mockParams['ambulancia.id'] = viagem.ambulancia.id
        mockParams['motorista.id'] = viagem.motorista.id

        mockParams.dataSaida = FormatadorDataHora.default.dateFormat.format(viagem.dataSaida)
        mockParams.horaSaida = FormatadorDataHora.default.timeFormat.format(viagem.horaSaida)
        mockParams.kmSaida = viagem.kmSaida

        mockParams.dataRetorno = FormatadorDataHora.default.dateFormat.format(viagem.dataRetorno)
        mockParams.horaRetorno = FormatadorDataHora.default.timeFormat.format(viagem.horaRetorno)
        mockParams.kmSaida = 131
        controller.update()

        sessionFactory.currentSession.clear()
        

        viagem = Viagem.get(viagem.id)
        assertEquals 131, viagem.kmSaida
    }

    void testeRegistroSaida() {
        GrailsMock mock = mockFor(GerenciamentoGrupoService)
		mock.demand.getOperadorLogado(1..1) { -> return operador }
        controller.gerenciamentoGrupoService = mock.createMock()

        mockParams['ambulancia.id'] = ambulancia.id
        mockParams['motorista.id'] = motorista.id
        mockParams.dataSaida = FormatadorDataHora.default.dateFormat.format(new Date())
        mockParams.horaSaida = FormatadorDataHora.default.timeFormat.format(new Date())
        mockParams.kmSaida = 12
        mockParams.observacoes = ''

        controller.saveSaida()

        println mockResponse.dump()
        sessionFactory.currentSession.clear()

        assertEquals 1, Viagem.count()

        Viagem viagem = Viagem.list().first()
        assertEquals ambulancia.placa, viagem.ambulancia.placa
        assertEquals motorista.nome, viagem.motorista.nome
        assertEquals 12, viagem.kmSaida

        assertNull viagem.horaRetorno
        assertNull viagem.dataRetorno        
    }

    void testeRegistreRetorno() {
        Viagem viagem = registreViagemComSaida(1000)

        GrailsMock mock = mockFor(GerenciamentoGrupoService)
		mock.demand.getOperadorLogado(1..1) { -> return operador }
        controller.gerenciamentoGrupoService = mock.createMock()

        mockParams.id = viagem.id
        mockParams.dataRetorno = FormatadorDataHora.default.dateFormat.format(new Date() + 1)
        mockParams.horaRetorno = FormatadorDataHora.default.timeFormat.format(new Date())
        mockParams.kmRetorno = 1012

        controller.updateRetorno()

        println mockResponse.dump()
        sessionFactory.currentSession.clear()

        viagem = Viagem.get(viagem.id)
        assertEquals 1012, viagem.kmRetorno
        assertTrue viagem.retornou
        assertEquals operador.usuario, viagem.operadorRetorno.usuario
    }
}

