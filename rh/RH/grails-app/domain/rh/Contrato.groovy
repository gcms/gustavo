package rh

class Contrato {
    Date dataAdmissao
    Date dataRegistro
    Date dataDemissao

    Cargo cargo
    int cargaHoraria
    BigDecimal salarioBase

    static constraints = {
    }
}
