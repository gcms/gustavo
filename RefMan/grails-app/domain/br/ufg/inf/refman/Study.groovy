package br.ufg.inf.refman

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 12/04/2010
 * Time: 11:37:39
 * To change this template use File | Settings | File Templates.
 */
class Study {
    String title
    String authorsString
    Publication publication

    String description

    static hasMany = [results:SearchResult]
    List results
}
