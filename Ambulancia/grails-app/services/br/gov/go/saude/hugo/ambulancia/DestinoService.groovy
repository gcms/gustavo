package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

class DestinoService {
    SessionFactory sessionFactory

    List obtenhaDestinos() {
        log.debug "Obtendo destinos disponíveis..."
        String hql = """
    select distinct upper(p.destino)
    from Viagem v join v.paradas p
    where v.retornou = true
    order by upper(p.destino) 
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
    order by p.destino
"""

        sessionFactory.currentSession.createQuery(hql).list()

    }
}
