package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

class MotoristaService {
    SessionFactory sessionFactory

    List obtenhaMotoristasDisponiveis() {
        String hql = """
        from Motorista m
        where not exists (select 1 from Viagem v
                          where v.retornou = false and v.motorista = m)
"""
        sessionFactory.currentSession.createQuery(hql).list()
    }
}
