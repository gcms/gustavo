import rh.Cargo
import rh.Empregado
import rh.Pessoa
import rh.Sexo

class BootStrap {

    def init = { servletContext ->
        Cargo professor = new Cargo(nome: 'PROFFESSOR')
        professor.save()

        Pessoa gustavo = new Pessoa(nome: 'Gustavo Sousa', dataNascimento: Date.parse('dd/MM/yyyy', '12/12/1985'), sexo: Sexo.MASCULINO)
        gustavo.save()

        Empregado professorGustavo = new Empregado(cargo: professor, pessoa: gustavo)
        professorGustavo.save()

    }
    def destroy = {
    }
}
