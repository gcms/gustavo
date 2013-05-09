package rh

class Pessoa {
    String nome
    Date dataNascimento
    Sexo sexo

    String nomePai
    String nomeMae

    String codigoNaturalidade
    Boolean pne = false

    EstadoCivil estadoCivil

    String cpf
    String rg
    String pis

    static constraints = {
        nome blank: false
        nomePai nullable: true
        nomeMae nullable: true
        codigoNaturalidade nullable: true
        pne nullable: true
        estadoCivil nullable: true
        cpf nullable: true
        rg nullable: true
        pis nullable: true
    }
}
