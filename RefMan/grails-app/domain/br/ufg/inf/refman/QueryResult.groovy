package br.ufg.inf.refman

class QueryResult {
    static constraints = {
        name(nullable: false, blank: false)
    }

    Engine engine
    String name
    String description
    String url

    Date date = new Date()

    static hasMany = [results:SearchResult]
    static transients = [ 'resultCount' ]

    List results = []

    public QueryResult() {}
    public QueryResult(Engine engine, Collection results, String name, String description, String url) {
        this.engine = engine
        addResults(results)
        this.name = name
        this.description = description
        this.url = url
    }

    public void addResults(Collection results) {
        results.each { addResult(it) }
    }

    public void addResult(SearchResult sr) {
        sr.queryResult = this
        results.add(sr)
    }

    public int getResultCount() {
        results.size()
    }
}
