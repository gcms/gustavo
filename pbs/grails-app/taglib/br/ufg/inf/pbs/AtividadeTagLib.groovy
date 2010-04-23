package br.ufg.inf.pbs

class AtividadeTagLib {
    def cadaAtividade = { attr, body ->
        int nivel = 0

        List atividades = attr.atividades        
        imprima(atividades , nivel, attr, body)
    }


    private void imprima(List atividades, int nivel, attr, body) {
        atividades.each { Atividade at ->
            out << body(nivel: nivel, atividade: at)
            imprima(at.filhos, nivel + 1, attr, body)
        }
    }
}
