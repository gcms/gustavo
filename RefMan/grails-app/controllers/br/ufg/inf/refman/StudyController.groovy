package br.ufg.inf.refman

class StudyController {
    SearchResultService searchResultService

    def index = {
        [studies: Study.list()]
    }

    def newStudy = {
        SearchResult searchResult = SearchResult.get(params.id)

        session.searchResult = new HashMap(searchResult.properties)
        println session.searchResult.id
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
        session.similarResults.remove(params.item)
        session.selectedResults.add(params.item)
        params.each { k, v -> session.searchResult[k] = v }
        redirect(action: edit)
    }

    def removeItem = {
        session.selectedResults.remove(params.item)
        session.similarResults.add(params.item)
        params.each { k, v -> session.searchResult[k] = v }
        redirect(action: edit)
    }

    def save = {
        List results = (session.selectedResults + [session.searchResult.id]).collect { SearchResult.get(it) }
        results.each { println it }
        Study study = new Study(session.searchResult, results)
        study.save(flush: true)
        redirect(action: index)
    }
}
