package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.utilitario.UtilitarioDataHorario

class ViagemController {
    MotoristaService motoristaService
    AmbulanciaService ambulanciaService
    DestinoService destinoService

    GerenciamentoGrupoService gerenciamentoGrupoService
    GerenciamentoViagemService gerenciamentoViagemService

    UtilitarioDataHorario dataHora = UtilitarioDataHorario.default

    def index = {
        redirect(action: "home", params: params)
    }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [save: "POST", update: "POST", delete: "POST"]

    private Map obtenhaViagens(Map listParams) {
        Date dataInicio = dataHora.inicioDoDia(params.dataInicio ? dataHora.dateFormat.parse(params.dataInicio) : dataHora.inicioDoMes())
        Date dataFim = dataHora.fimDoDia(params.dataFim ? dataHora.dateFormat.parse(params.dataFim) : new Date())

        Motorista motorista = params?.motorista?.id ? Motorista.get(params.motorista.id) : null
        Ambulancia ambulancia = params?.ambulancia?.id ? Ambulancia.get(params.ambulancia.id) : null
        String destino = params.getInt('destino') != 0 ? params.destino : null

        def criteria = {
            and {
                between("horaSaida", dataInicio, dataFim)
                between("horaRetorno", dataInicio, dataFim)
                eq("retornou", true)

                if (motorista)
                    eq("motorista.id", motorista.id)

                if (ambulancia)
                    eq("ambulancia.id", ambulancia.id)

                if (destino)
                    paradas {
                        eq('destino', destino)
                    }
            }
            projections {
                sum("distancia")
            }
        }

        def viagens = Viagem.createCriteria().list(listParams, criteria)
        def distanciaTotal = Viagem.createCriteria().get(criteria)

        [
                motorista: motorista, ambulancia: ambulancia, destino: destino,
                dataInicio: dataInicio, dataFim: dataFim,
                viagens: viagens, distanciaTotal: distanciaTotal
        ]
    }

    def list = {
        [
                motoristas: Motorista.findAllByDisponivel(true),
                ambulancias: Ambulancia.list(),
                destinos: destinoService.obtenhaDestinos()
        ] + obtenhaViagens(params)
    }

    def print = {
        obtenhaViagens([:])
    }

    def home = {
        List viagens = Viagem.createCriteria().list(params) { eq('retornou', false) }

        [viagens: viagens]
    }

    def create = {
        List motoristas = motoristaService.obtenhaMotoristasDisponiveis()
        List ambulancias = ambulanciaService.obtenhaAmbulanciasDisponiveis()

        params.horaSaida = params.horaSaida ?: new Date()
        params.dataSaida = params.dataSaida ?: new Date()
        Viagem viagem = flash.viagem ?: new Viagem(params)

        [viagem: viagem, motoristas: motoristas, ambulancias: ambulancias, hoje: new Date()]
    }

    def save = {
        params.horaSaida = dataHora.timeFormat.parse(params.horaSaida)
        params.dataSaida = dataHora.dateFormat.parse(params.dataSaida)

        // TODO: tentar colocar em um binding customizado
        List paradas = []
        params.findAll { it.key =~ ~/^paradas\[(\d+)\]$/ }.each { key, val ->
            Class klass = getClass().getClassLoader().loadClass(val['class'])
            paradas.add(klass.newInstance(val))
        }

        Viagem viagem = new Viagem(params)
        viagem.operador = gerenciamentoGrupoService.operadorLogado

        // TODO: tentar colocar em um binding customizado
        viagem.paradas.clear()
        viagem.paradas.addAll(paradas)

        if (gerenciamentoViagemService.registreSaida(viagem)) {
            flash.message = "viagem.created"
            flash.args = [viagem.id]
            flash.defaultMessage = "Viagem registrada com o identificador ${viagem.id}"
            redirect(action: 'home')
        } else {
            flash.viagem = viagem
            redirect(action: "create")
        }
    }

    def show = {
        def viagem = Viagem.get(params.id)
        if (!viagem) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "list")
        } else {
            return [viagem: viagem]
        }
    }

    // TODO: diferenciar ediçãoo de retorno
    def edit = {
        def viagemBanco = Viagem.get(params.id)
        def viagem = flash.viagem ?: viagemBanco

        viagem.dataRetorno = viagem.dataRetorno ?: [new Date(), viagem.dataSaida].max()
        viagem.horaRetorno = viagem.horaRetorno ?: [new Date(), viagem.horaSaida].max()

        if (!viagem) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "list")
        }
        else {
            return [viagem: viagem, viagemBanco: viagemBanco]
        }
    }

    // TODO: diferenciar confirmação de retorno de atualização
    def update = {
        params.horaRetorno = dataHora.timeFormat.parse(params.horaRetorno)
        params.dataRetorno = dataHora.dateFormat.parse(params.dataRetorno)

        def viagem = Viagem.get(params.id)

        if (viagem) {
            if (viagem.retornou) {
                flash.message = 'viagem.not.deleted'
                flash.args = [params.id]
                flash.defaultMessage = "Viagem ${params.id} já retornou, não pode ser atualizada"

                redirect(action: 'show', id: params.id)
            }

            viagem.properties = params
            viagem.retornou = true

            if (gerenciamentoViagemService.registreRetorno(viagem)) {
                flash.message = "viagem.updated"
                flash.args = [params.id]
                flash.defaultMessage = "Retorno da viagem ${params.id} foi registrado"
                redirect(action: "show", id: viagem.id)
            } else {
                flash.viagem = viagem
                redirect(action: 'edit', id: params.id)
            }
        }
        else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def delete = {
        def viagem = Viagem.get(params.id)

        if (viagem) {
            if (viagem.retornou) {
                flash.message = 'viagem.not.deleted'
                flash.args = [params.id]
                flash.defaultMessage = "Viagem ${params.id} já retornou, não pode ser cancelada"

                redirect(action: 'show', id: params.id)
            }

            try {
                viagem.delete()
                flash.message = "viagem.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem com identificador ${params.id} foi excluída"
                redirect(action: "home")
            }
            catch (org.springframework.dao.DataIntegrityViolationException e) {
                flash.message = "viagem.not.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem com identificador ${params.id} não pode ser excluída"
                redirect(action: "show", id: params.id)
            }
        }
        else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "home")
        }
    }
}
