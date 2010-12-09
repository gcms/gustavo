package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

class DestinoService {
    SessionFactory sessionFactory

    List obtenhaDestinos() {
        log.debug "Obtendo destinos disponíveis..."
        String hql = """
    select distinct p.destino
    from Viagem v join v.paradas p
    where v.retornou = true 
"""

        sessionFactory.currentSession.createQuery(hql).list()
    }

    List obtenhaDestinosLike(String nome) {
        log.debug "Obtendo destinos like '$nome'..."

        String hql = """
    select distinct p.destino
    from Viagem v join v.paradas p
    where v.retornou = true
    and upper(p.destino) like upper('%$nome%') 
"""

        sessionFactory.currentSession.createQuery(hql).list()

    }
}
