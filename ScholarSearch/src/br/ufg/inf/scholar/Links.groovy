package br.ufg.inf.scholar

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:45:01
 * To change this template use File | Settings | File Templates.
 */
class Links {
    public int qtd
    public String citedBy
    public String related
    public String versions

    public String toString() {
        "Cited by ${qtd}\n"
    }
}