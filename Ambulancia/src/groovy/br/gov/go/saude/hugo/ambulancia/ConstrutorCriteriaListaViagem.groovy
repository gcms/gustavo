package br.gov.go.saude.hugo.ambulancia

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 09/12/2010
 * Time: 12:12:18
 * To change this template use File | Settings | File Templates.
 */
class ConstrutorCriteriaListaViagem {
    private Date dataInicio
    private Date dataFim

    private Motorista motorista
    private Ambulancia ambulancia

    private String destino

    public Closure obtenhaCriteria() {
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

        criteria
    }
}
