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
        title(nullable: false, blank: false)
        year(nullable: true)
        description(nullable: true)
    }

    static hasMany = [results:SearchResult, authors:String]
    static transients = [ 'authorsString' ]

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
    List authors = []
    Integer year
    String description

    int citationCount = -1

    Set results = [] as Set

    public void setAuthorsString(String authorsString) {
        authors = (authorsString.split(',') as List).collect { it.trim() }
    }

    public String getAuthorsString() {
        authors.join(', ')
    }
}