package br.gov.go.saude.hugo.ambulancia.importacao

import br.gov.go.saude.hugo.utilitario.LevenshteinDistance
import br.gov.go.saude.hugo.ambulancia.Motorista

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 06/12/2010
 * Time: 07:48:23
 * To change this template use File | Settings | File Templates.
 */
class GerenciadorMotoristas {
    private static final int MAX_DISTANCE = 3

    public GerenciadorMotoristas() {
        Motorista.list().each { Motorista motorista ->
            adicione(null, motorista)
        }
    }

    List motoristas = []
    Map principais = [:]
    Map similares = [:]

    String trateNome(String nome) {
        nome.replaceAll('\\s', '').toUpperCase()
    }

    public Motorista obtenha(String nome) {
        encontre(nome)
    }

    private Motorista encontre(String nome) {
        Motorista obtida = motoristas.find { Motorista motorista ->
            ehIgual(motorista, nome)
        }

        similares[obtida]
    }

    public Motorista adicione(Motorista encontrado, Motorista novo) {
        motoristas.add(novo)

        if (encontrado == null) {
            principais[novo] = []
        } else {
            principais[encontrado].add(novo)
        }
        similares[novo] = encontrado ?: novo
    }

    private boolean ehIgual(Motorista motorista, String nome) {
        LevenshteinDistance.calcule(trateNome(motorista.nome), trateNome(nome)) <= MAX_DISTANCE
    }

    public boolean ehPrincipal(Motorista motorista) {
        principais[motorista] != null
    }
}
