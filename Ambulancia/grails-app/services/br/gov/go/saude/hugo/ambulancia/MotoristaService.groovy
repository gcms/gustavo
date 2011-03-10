package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

class MotoristaService {
    SessionFactory sessionFactory

    List obtenhaMotoristasDisponiveis() {
        log.debug 'Obtendo motoristas disponíveis...'
        
        String hql = """
        from Motorista m
        where m.disponivel = true
        and not exists (select 1 from Viagem v
                          where v.retornou = false and v.motorista = m)
        order by nome
"""
        sessionFactory.currentSession.createQuery(hql).list()
    }
}
