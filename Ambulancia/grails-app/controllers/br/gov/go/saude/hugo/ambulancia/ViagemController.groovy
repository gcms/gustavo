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
        List viagens = Viagem.createCriteria().list(params) { eq('retornou', false) }

        [viagens: viagens]
    }

    // the delete, save and update actions only accept POST requests
    static allowedMethods = [saveSaida: "POST", updateRetorno: "POST", deleteSaida: "POST", update: 'POST', delete: 'POST']

    private Map obtenhaViagens(Map listParams) {
        Date dataInicio = dataHora.inicioDoDia(params.dataInicio ? dataHora.dateFormat.parse(params.dataInicio) : dataHora.inicioDoMes())
        Date dataFim = dataHora.fimDoDia(params.dataFim ? dataHora.dateFormat.parse(params.dataFim) : new Date())

        Motorista motorista = params?.motorista?.id ? Motorista.get(params.motorista.id) : null
        Ambulancia ambulancia = params?.ambulancia?.id ? Ambulancia.get(params.ambulancia.id) : null
        String destino = params.getInt('destino') != 0 ? params.destino : null

        def criteria = new ConstrutorCriteriaListaViagem(dataInicio: dataInicio, dataFim: dataFim,
                motorista: motorista, ambulancia: ambulancia, destino: destino).obtenhaCriteria()

        def viagens = Viagem.createCriteria().list(listParams, criteria)
        def distanciaTotal = Viagem.createCriteria().get(criteria)

        [
                motorista: motorista, ambulancia: ambulancia, destino: destino,
                dataInicio: dataInicio, dataFim: dataFim,
                viagens: viagens, distanciaTotal: distanciaTotal
        ]
    }

    def list = {
        Map opcoes = [
                motoristas: Motorista.list(),
                ambulancias: Ambulancia.list(),
                destinos: destinoService.obtenhaDestinos()
        ]

        obtenhaViagens(params) + opcoes
    }

    def print = {
        obtenhaViagens([:])
    }

    def createSaida = {
        List motoristas = motoristaService.obtenhaMotoristasDisponiveis()
        List ambulancias = ambulanciaService.obtenhaAmbulanciasDisponiveis()

        params.horaSaida = params.horaSaida ?: new Date()
        params.dataSaida = params.dataSaida ?: new Date()
        Viagem viagem = flash.viagem ?: new Viagem(params)

        [viagem: viagem, motoristas: motoristas, ambulancias: ambulancias, hoje: new Date()]
    }

    def saveSaida = {
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
            redirect(action: 'index')
        } else {
            flash.viagem = viagem
            redirect(action: "createSaida")
        }
    }

    def showSaida = {
        def viagem = Viagem.get(params.id)
        if (!viagem) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "index")
        } else if (viagem.retornou) {
            redirect(action: "show", params: params)
        } else {
            return [viagem: viagem]
        }
    }

    def editRetorno = {
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

    def updateRetorno = {
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
                redirect(action: 'editRetorno', id: params.id)
            }
        }
        else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def deleteSaida = {
        def viagem = Viagem.get(params.id)

        if (viagem && viagem.retornou) {
            flash.message = 'viagem.not.deleted'
            flash.args = [params.id]
            flash.defaultMessage = "Viagem ${params.id} já retornou, não pode ser cancelada"

            redirect(action: 'showSaida', id: params.id)
        } else if (viagem) {
            try {
                viagem.delete()
                flash.message = "viagem.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem com identificador ${params.id} foi excluída"
                redirect(action: "index")
            }
            catch (org.springframework.dao.DataIntegrityViolationException e) {
                flash.message = "viagem.not.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem com identificador ${params.id} não pode ser excluída"
                redirect(action: "showSaida", id: params.id)
            }
        } else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "index")
        }
    }

    def show = {
        def viagem = Viagem.get(params.id)
        if (!viagem) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Não foi encontrada viagem com identificador ${params.id}"
            redirect(action: "index")
        } else if (!viagem.retornou) {
            redirect(action: "showSaida", params: params)
        } else {
            return [viagem: viagem]
        }
    }

    def edit = {
        def viagem = flash.viagem ?: Viagem.get(params.id)
        if (!viagem) {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Viagem not found with id ${params.id}"
            redirect(action: "index")
        }
        else {
            return [viagem: viagem]
        }
    }

    def update = {
        params.horaSaida = dataHora.timeFormat.parse(params.horaSaida)
        params.dataSaida = dataHora.dateFormat.parse(params.dataSaida)
        params.horaRetorno = dataHora.timeFormat.parse(params.horaRetorno)
        params.dataRetorno = dataHora.dateFormat.parse(params.dataRetorno)

        def viagem = Viagem.get(params.id)
        if (viagem) {
            if (params.version) {
                def version = params.version.toLong()
                if (viagem.version > version) {

                    viagem.errors.rejectValue("version", "viagem.optimistic.locking.failure", "Another user has updated this Viagem while you were editing")
                    render(view: "edit", model: [viagemInstance: viagem])
                    return
                }
            }
            viagem.properties = params
            if (!viagem.hasErrors() && viagem.save()) {
                flash.message = "viagem.updated"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem ${params.id} updated"
                redirect(action: "show", id: viagem.id)
            }
            else {
                flash.viagem = viagem
                redirect(action: "edit")
            }
        }
        else {
            flash.message = "viagem.not.found"
            flash.args = [params.id]
            flash.defaultMessage = "Viagem not found with id ${params.id}"
            redirect(action: "edit", id: params.id)
        }
    }

    def delete = {
        def viagem = Viagem.get(params.id)

        if (viagem) {
            try {
                viagem.delete()
                flash.message = "viagem.deleted"
                flash.args = [params.id]
                flash.defaultMessage = "Viagem com identificador ${params.id} foi excluída"
                redirect(action: "index")
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
            redirect(action: "index")
        }
    }
}
