package rh

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 09/04/13
 * Time: 10:51
 * To change this template use File | Settings | File Templates.
 */
enum Sexo {
    MASCULINO('M'), FEMININO('F');

    private String desc
    Sexo(String desc) {
        this.desc = desc
    }

    public String toString() {
        desc
    }
}
