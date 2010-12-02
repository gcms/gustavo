package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

class DestinoService {
    SessionFactory sessionFactory

    List obtenhaDestinos() {
        String hql = """
    select distinct p.destino
    from Viagem v join v.paradas p
    where v.retornou = true 
"""

        sessionFactory.currentSession.createQuery(hql).list()
    }
}
