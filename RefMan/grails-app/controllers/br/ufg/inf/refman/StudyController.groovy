package br.ufg.inf.refman

class StudyController {
    SearchResultService searchResultService

    def index = { }

    def newStudy = {
        SearchResult searchResult = SearchResult.get(params.id)

        session.searchResult = new HashMap(searchResult.properties)
        session.similarResults = searchResultService.getSimilarResults(searchResult).collect { it.id.toString() }
        session.selectedResults = []

        redirect(action: edit)
    }

    def edit = {
        [searchResult: session.searchResult,
                selectedResults: session.selectedResults.collect { SearchResult.get(it) },
                similarResults: session.similarResults.collect { SearchResult.get(it) }]
    }

    def addItem = {
        session.similarResults.remove(params.id)
        session.selectedResults.add(params.id)
        params.each { k, v -> session.searchResult[k] = v }
        redirect(action: edit)
    }

    def removeItem = {
        session.selectedResults.remove(params.id)
        session.similarResults.add(params.id)
        params.each { k, v -> session.searchResult[k] = v }
        redirect(action: edit)
    }
}
