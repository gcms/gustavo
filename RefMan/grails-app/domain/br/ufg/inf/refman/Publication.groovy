package br.ufg.inf.refman

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 11:39:22
 * To change this template use File | Settings | File Templates.
 */
class Publication {
    String name
    Integer year

    public String toString() {
        "${name}, ${year}"
    }
}
