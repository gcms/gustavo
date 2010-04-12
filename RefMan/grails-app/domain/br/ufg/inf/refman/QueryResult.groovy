package br.ufg.inf.refman

class QueryResult {
    static constraints = {
        name(nullable: false, blank: false)
    }

    Engine engine
    String name
    String description

    static hasMany = [results:SearchResult]
    List results = []

    public QueryResult() {}
    public QueryResult(Engine engine, Collection results, String name, String description) {
        this.engine = engine
        addResults(results)
        this.name = name
        this.description = description
    }

    public void addResults(Collection results) {
        results.each { addResult(it) }
    }

    public void addResult(SearchResult sr) {
        sr.queryResult = this
        results.add(sr)
    }
}
