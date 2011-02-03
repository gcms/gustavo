package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory
import org.hibernate.Query

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
        log.debug "Obtendo km da ambulância $ambulancia..."

        String hql = """
        select max(kmRetorno) from Viagem v
        where v.retornou = true and v.ambulancia.id = :id)
"""
        Query query = sessionFactory.currentSession.createQuery(hql)
        query.setParameter('id', ambulancia.id)

        query.uniqueResult() ?: 0
    }

    Long obtenhaKmRetornoUltimaViagem(Ambulancia ambulancia, Date dataHora) {
        log.debug "Obtendo km da ambulância $ambulancia em $dataHora..."

        String hql = """
        select max(kmRetorno) from Viagem v
        where v.retornou = true
        and v.ambulancia.id = :id
        and v.horaRetorno < :dataHora)
"""
        Query query = sessionFactory.currentSession.createQuery(hql)
        query.setParameter('id', ambulancia.id)
        query.setParameter('dataHora', dataHora)

        query.uniqueResult() ?: 0
    }
}
