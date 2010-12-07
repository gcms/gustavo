package br.gov.go.saude.hugo.ambulancia.importacao

import br.gov.go.saude.hugo.ambulancia.Viagem
import br.gov.go.saude.hugo.ambulancia.Ambulancia
import br.gov.go.saude.hugo.ambulancia.Operador
import br.gov.go.saude.hugo.ambulancia.Motorista

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 06/12/2010
 * Time: 11:55:45
 * To change this template use File | Settings | File Templates.
 */
class ImportadorPlanilhaBanco {
    File caminho = new File("/d:/documents and settings/gustavosousa/Desktop/")

    ImportadorPlanilha importador = new ImportadorPlanilha()

    void importeArquivos(List meses) {
//            List meses = [/*'AGOSTO', 'SETEMBRO', 'OUTUBRO',*/ 'NOVEMBRO', 'DEZEMBRO']

        meses.each { mes ->
            def is = new FileInputStream(new File(caminho, "${mes}.xls"))
            List itens = importador.importeRegistros(is)

            for (int i = 0; i < itens.size(); i++) {
                if (i > 0) {
                    if (itens[i].kmSaida == null)
                        itens[i].kmSaida = itens[i-1].kmRetorno
                }


                if (i < itens.size() - 1) {
                    if (itens[i].kmRetorno == null || itens[i].kmRetorno < itens[i].kmSaida || itens[i].kmRetorno - itens[i].kmSaida >= 100) {
                        itens[i].kmRetorno = itens[i+1].kmSaida
                        if (itens[i].kmRetorno == null || itens[i].kmRetorno < itens[i].kmSaida || itens[i].kmRetorno - itens[i].kmSaida >= 100) {
                            itens[i].kmRetorno = itens[i].kmSaida
                        }
                    }
                }

//                assert itens[i].kmSaida != null && itens[i].kmRetorno != null
            }

            Viagem.withTransaction { status ->
                try {
                    itens.eachWithIndex { item, idx ->
                        println item
                        salveItem(itens, idx)
                    }
                } catch (Exception e) {
                    status.setRollbackOnly()
                    throw e
                }
            }
        }
    }

    private Integer encontreKmRetorno(List itens, Ambulancia amb) {
        itens.reverse().find { it.ambulancia.placa == amb.placa }?.kmRetorno
    }

    private Integer encontreKmSaida(List itens, Ambulancia amb) {
        itens.find { it.ambulancia.placa == amb.placa }?.kmSaida
    }

    void salveItem(List itens, int idx) {
        Map item = itens[idx]

        Operador operador = item.operador
        if (operador.id == null) {
            println "Saving.. $operador"
            operador.usuario = operador.nome.replaceAll('\\s', '').toLowerCase()
            operador.senha = '12345'

            operador.validate()
            operador.errors.allErrors.each { println it }

            operador.save()
//            OperadorGrupo.create operador, Grupo.findByAuthority('ROLE_USER')
        }

        println "OperadorID: ${operador.id}"

        Motorista motorista = item.motorista
        if (motorista.id  == null) {
            motorista.validate()
            motorista.errors.allErrors.each { println it }
            
            motorista.save()
        }

        Ambulancia ambulancia = item.ambulancia
        if (ambulancia.id == null) {

            ambulancia.validate()
            ambulancia.errors.allErrors.each { println it }

            ambulancia.save()
        }

        Viagem viagem = new Viagem(item)


//        println "ItemSaida: ${item.kmSaida}/ItemRetorno: ${item.kmRetorno}"
//        if (item.kmSaida == null || item.kmSaida >= item.kmRetorno && idx > 0) {
//            println "Searching kmSaida"
//            item.kmSaida = encontreKmRetorno(itens[0..(idx-1)], item.ambulancia, item.kmSaida ?: Integer.MAX_VALUE) ?: item.kmSaida ?: 0
//        }
//
//        viagem.kmSaida = item.kmSaida //? item.kmSaida.toInteger() : 0
//
//        if (item.kmRetorno == null || item.kmSaida >= item.kmRetorno && idx < itens.size() - 1) {
//            println "Searching kmRetorno"
//            item.kmRetorno = encontreKmSaida(itens[(idx+1)..(itens.size() - 1)], item.ambulancia, item.kmRetorno ?: item.kmSaida) ?: item.kmRetorno
//        }

        if (item.kmSaida == null && idx > 0 && idx < itens.size() -1) {
            item.kmSaida = encontreKmRetorno(itens[0..(idx-1)], item.ambulancia)
            item.kmRetorno = encontreKmSaida(itens[(idx+1)..(itens.size()-1)], item.ambulancia)

            if (item.kmSaida == null) {
                item.kmSaida = 0
            }
            if (item.kmRetorno == null) {
                item.kmRetorno = item.kmSaida
            }
        } else if (item.kmSaida == null) {
            item.kmSaida = 0
            item.kmRetorno = item.kmSaida
        } else if (item.kmRetorno == null || item.kmSaida > item.kmRetorno) {
            item.kmRetorno = item.kmSaida
        }

        viagem.kmSaida = item.kmSaida
        viagem.kmRetorno = item.kmRetorno

        if (viagem.kmRetorno < viagem.kmSaida) {
            println 'KM: ' + 'Saida: ' + viagem.kmSaida + ' Retorno: ' + viagem.kmRetorno
        }

        if (viagem.horaRetorno < viagem.horaSaida) {
            Calendar cal = Calendar.instance
            cal.time = viagem.horaSaida

            long horaSaida = cal.get(Calendar.HOUR_OF_DAY)

            cal.time = viagem.horaRetorno
            long horaRetorno = cal.get(Calendar.HOUR_OF_DAY)
            long minutoRetorno = cal.get(Calendar.MINUTE)

            if ((horaRetorno != 0 && minutoRetorno != 0) &&
                    horaSaida > 12 && horaRetorno < 12) {
                cal.time = viagem.dataSaida
                cal.add(Calendar.DAY_OF_MONTH, 1)
                viagem.dataRetorno = cal.time
            } else {
                viagem.horaRetorno = viagem.horaSaida
            }
        }

        if (viagem.horaRetorno < viagem.horaSaida) {
            println 'HORA:' + 'Saida: ' + viagem.horaSaida + ' Retorno: ' + viagem.horaRetorno
        }

        if (viagem.kmRetorno - viagem.kmSaida >= 100) {
            println 'GRANDE: '  + 'Distancia: '  + (viagem.kmRetorno - viagem.kmSaida)
        }


        viagem.retornou = true

        viagem.validate()
        if (viagem.hasErrors())
            viagem.errors.allErrors.each { println it }
        viagem.save()
    }

}
