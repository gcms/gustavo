import rh.Cargo
import rh.Contrato
import rh.Empregado
import rh.Escolaridade
import rh.Formacao
import rh.Pessoa
import rh.Sexo

class BootStrap {

    def init = { servletContext ->
        Cargo professor = new Cargo(nome: 'PROFFESSOR')
        professor.save()

        Pessoa gustavo = new Pessoa(nome: 'Gustavo Sousa', dataNascimento: Date.parse('dd/MM/yyyy', '12/12/1985'), sexo: Sexo.MASCULINO)
        gustavo.save()

        Formacao formacao = new Formacao(nivel: Escolaridade.SUPERIOR, descricao: 'LICENCIATURA EM BIOLOGIA')
        formacao.save()

        Empregado professorGustavo = new Empregado(pessoa: gustavo, formacao: formacao)
        professorGustavo.contrato = new Contrato(cargo: professor)
        professorGustavo.save()

    }
    def destroy = {
    }
}
