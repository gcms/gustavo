package br.gov.go.saude.hugo.ambulancia.importacao

import br.gov.go.saude.hugo.utilitario.LevenshteinDistance
import br.gov.go.saude.hugo.ambulancia.Operador

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 06/12/2010
 * Time: 07:48:23
 * To change this template use File | Settings | File Templates.
 */
class GerenciadorOperadores {
    private static final int MAX_DISTANCE = 3
    
    public GerenciadorOperadors() {
        Operador.list().each { Operador operador ->
            adicione(null, operador)
        }
    }

    List operadores = []
    Map principais = [:]
    Map similares = [:]

    String trateNome(String nome) {
        nome.replaceAll('\\s', '').toUpperCase()
    }

    public Operador obtenha(String nome) {
        encontre(nome)
    }

    private Operador encontre(String nome) {
        Operador obtida = operadores.find { Operador operador ->
            ehIgual(operador, nome)
        }

        similares[obtida]
    }

    public Operador adicione(Operador encontrado, Operador novo) {
        operadores.add(novo)

        if (encontrado == null) {
            principais[novo] = []
        } else {
            principais[encontrado].add(novo)
        }
        similares[novo] = encontrado ?: novo
    }

    private boolean ehIgual(Operador operador, String nome) {
        LevenshteinDistance.calcule(trateNome(operador.nome), trateNome(nome)) <= MAX_DISTANCE
    }

    public boolean ehPrincipal(Operador operador) {
        principais[operador] != null
    }
}
