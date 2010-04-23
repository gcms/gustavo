import br.ufg.inf.pbs.Atividade
import br.ufg.inf.pbs.Workflow
import br.ufg.inf.pbs.Projeto
import grails.util.Environment
import br.ufg.inf.pbs.Utils

class BootStrap {

    def init = {servletContext ->
        if (Environment.current != Environment.TEST) {
            Projeto projeto = Utils.obtenhaProdutoInicializado()
            projeto.save(flush: true)
        }

    }
    def destroy = {
    }
} 