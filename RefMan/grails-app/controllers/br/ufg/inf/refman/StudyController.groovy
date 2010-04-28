package br.ufg.inf.refman

class StudyController {
    SearchResultService searchResultService

    def index = {
        [studies: Study.list()]
    }

    private void initSession(Map fields, String action, List similarResults, List selectedResults) {
        session.fields = new HashMap(fields)
        session.fields.action = action
        session.fields.year = fields.year ?: fields.publicationYear 

        session.similarResults = similarResults.collect { it.id.toString() }
        session.selectedResults = selectedResults.collect { it.id.toString() }
    }

    private List getResults(List ids) {
        ids.collect { SearchResult.get(it) }
    }

    def newStudy = {
        SearchResult searchResult = SearchResult.get(params.id)

        List similarResults = searchResultService.getSimilarResults(searchResult)
        initSession(searchResult.properties, 'save', similarResults, [])

        redirect(action: edit)
    }

    def edit = {
        [fields: session.fields,
                selectedResults: getResults(session.selectedResults),
                similarResults: getResults(session.similarResults)]
    }

    def addItem = {
        session.similarResults.remove(params.item)
        session.selectedResults.add(params.item)
        params.each { k, v -> session.fields[k] = v }
        redirect(action: edit)
    }

    def removeItem = {
        session.selectedResults.remove(params.item)
        session.similarResults.add(params.item)
        params.each { k, v -> session.fields[k] = v }
        redirect(action: edit)
    }

    def save = {
        List results = getResults(session.selectedResults + [session.fields.id])
        Study study = new Study(session.fields, results)
        study.citationCount = params.citationCount.toInteger()
        study.save(flush: true)

        redirect(action: view, id: study.id)
    }

    def update = {
        Study study = Study.get(params.id)
        study.properties = params
        study.citationCount = params.citationCount.toInteger()
      
        study.results.removeAll(getResults(session.similarResults))
        study.results.addAll(getResults(session.selectedResults))
        study.save(flush: true)

        redirect(action: view, id: study.id)
    }

    def view = {
        Study study = Study.get(params.id)

        println study.results.size()
        List similarResults = searchResultService.getSimilarResults(study.results.iterator().next())
        initSession(study.properties, 'update', similarResults, study.results as List)

        redirect(action: edit)
    }
}