package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 18/11/2010
 * Time: 10:25:57
 * To change this template use File | Settings | File Templates.
 */
class AmbulanciaService {
    SessionFactory sessionFactory

    List obtenhaAmbulanciasDisponiveis() {
        log.debug "Obtendo ambulâncias disponíveis..."

        String hql = """
        from Ambulancia a
        where a.disponivel = true
          and not exists (select 1 from Viagem v
                          where v.retornou = false and v.ambulancia = a)
"""

        Ambulancia.executeQuery(hql)
    }

    Long obtenhaKmRetornoUltimaViagem(Ambulancia ambulancia) {
        log.debug "Obtendo km da ambulancia $ambulancia..."

        String hql = """
        select max(kmRetorno) from Viagem v
        where v.retornou = true and v.ambulancia.id = ${ambulancia.id})
"""
        sessionFactory.currentSession.createQuery(hql).uniqueResult() ?: 0
    }
}
