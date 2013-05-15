package rh

class Contrato {
    Date dataAdmissao
    Date dataRegistro
    Date dataDemissao

    BigDecimal cargaHoraria
    BigDecimal salarioBase

    Cargo cargo

    static belongsTo = Empregado

    static constraints = {
        dataAdmissao nullable: true
        dataRegistro nullable: true
        dataDemissao nullable: true
        cargaHoraria nullable: true
        salarioBase nullable: true
    }
}
