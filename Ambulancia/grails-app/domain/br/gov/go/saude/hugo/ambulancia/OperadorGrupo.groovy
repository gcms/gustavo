package br.gov.go.saude.hugo.ambulancia

import org.apache.commons.lang.builder.HashCodeBuilder

class OperadorGrupo implements Serializable {

	Operador operador
	Grupo grupo

	boolean equals(other) {
		if (!(other instanceof OperadorGrupo)) {
			return false
		}

		other.operador?.id == operador?.id &&
			other.grupo?.id == grupo?.id
	}

	int hashCode() {
		def builder = new HashCodeBuilder()
		if (operador) builder.append(operador.id)
		if (grupo) builder.append(grupo.id)
		builder.toHashCode()
	}

	static OperadorGrupo get(long operadorId, long grupoId) {
		find 'from OperadorGrupo where operador.id=:operadorId and grupo.id=:grupoId',
			[operadorId: operadorId, grupoId: grupoId]
	}

	static OperadorGrupo create(Operador operador, Grupo grupo, boolean flush = false) {
		new OperadorGrupo(operador: operador, grupo: grupo).save(flush: flush, insert: true)
	}

	static boolean remove(Operador operador, Grupo grupo, boolean flush = false) {
		OperadorGrupo instance = OperadorGrupo.findByOperadorAndGrupo(operador, grupo)
		instance ? instance.delete(flush: flush) : false
	}

	static void removeAll(Operador operador) {
		executeUpdate 'DELETE FROM OperadorGrupo WHERE operador=:operador', [operador: operador]
	}

	static void removeAll(Grupo grupo) {
		executeUpdate 'DELETE FROM OperadorGrupo WHERE grupo=:grupo', [grupo: grupo]
	}

	static mapping = {
		id composite: ['grupo', 'operador']
		version false
	}
}
