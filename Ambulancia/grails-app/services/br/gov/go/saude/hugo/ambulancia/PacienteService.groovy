package br.gov.go.saude.hugo.ambulancia

import org.hibernate.SessionFactory

class PacienteService {
    SessionFactory sessionFactory

    List obtenhaPacientes() {
        log.debug "Obtendo pacientes disponíveis..."
        String hql = """
    select distinct upper(p.paciente)
    from Viagem v join v.paradas p
    where v.retornou = true and p.class = ParadaPaciente
    order by upper(p.paciente) 
"""

        sessionFactory.currentSession.createQuery(hql).list()
    }
}
