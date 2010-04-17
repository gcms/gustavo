package br.ufg.inf.refman

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 12/04/2010
 * Time: 11:37:39
 * To change this template use File | Settings | File Templates.
 */
class Study {
    static constraints = {
        year(nullable: true)
        description(nullable: true)
    }

    public Study() {}
    public Study(Map params, Collection results) {
        this.properties = params
        results.each { addResult(it) }
    }

    public void addResult(SearchResult result) {
        result.study = this
        results.add(result)
    }

    String title
    String authorsString
    Integer year
    String description

    int citationCount = -1

    static hasMany = [results:SearchResult]
    Set results = [] as Set
}