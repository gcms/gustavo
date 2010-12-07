package br.gov.go.saude.hugo.ambulancia.importacao

import br.gov.go.saude.hugo.utilitario.LevenshteinDistance
import br.gov.go.saude.hugo.ambulancia.Ambulancia

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 06/12/2010
 * Time: 07:48:23
 * To change this template use File | Settings | File Templates.
 */
class GerenciadorAmbulancias {
    private static final int MAX_DISTANCE = 1

    List ambulancias = []
    Map principais = [:]
    Map similares = [:]

    public GerenciadorAmbulancias() {
        Ambulancia.list().each {
            adicione(null, it)
        }
    }


    String tratePrefixo(String prefixo) {
        prefixo.replaceAll('\\s', '').toUpperCase()
    }

    String tratePlaca(String placa) {
        placa.replaceAll('\\W', '').toUpperCase()
    }

    public Ambulancia obtenha(String prefixo, String placa) {
        encontre(prefixo, placa)
    }

    private Ambulancia encontre(String prefixo, String placa) {
        Ambulancia obtida = ambulancias.find { Ambulancia amb ->
            ehIgual(amb, prefixo, placa)
        }

        if (obtida == null && placa =~ /\d{4}/) {
            obtida = ambulancias.find { Ambulancia amb ->
                amb.placa.endsWith(placa)
            }
        }

        if (obtida == null) {
            obtida = ambulancias.find { Ambulancia amb ->
                tratePlaca(amb.placa) == tratePlaca(placa)
            }
        }

        if (obtida == null) {
            obtida = ambulancias.find { Ambulancia amb ->
                tratePrefixo(amb.prefixo) == tratePlaca(prefixo)
            }
        }

        similares[obtida]
    }

    public Ambulancia adicione(Ambulancia encontrada, Ambulancia nova) {
        ambulancias.add(nova)

        if (encontrada == null) {
            principais[nova] = []
        } else {
            principais[encontrada].add(nova)
        }
        similares[nova] = encontrada ?: nova
    }

    private boolean ehIgual(Ambulancia ambulancia, String prefixo, String placa) {
        LevenshteinDistance.calcule(tratePrefixo(ambulancia.prefixo), tratePrefixo(prefixo)) <= MAX_DISTANCE &&
                LevenshteinDistance.calcule(tratePlaca(ambulancia.placa), tratePlaca(placa)) <= MAX_DISTANCE
    }

    public boolean ehPrincipal(Ambulancia ambulancia) {
        principais[ambulancia] != null
    }
}
