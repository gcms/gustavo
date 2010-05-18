package br.ufg.inf.refman

class StudyController {
    SearchResultService searchResultService
    StudyService studyService

    def index = {
        [studies: Study.list().sort { -it.citationCount }]
    }

    private void initSession(Map fields, String action, List similarResults, List selectedResults) {
        session.fields = new HashMap(fields)
        session.fields.acao = action
        session.fields.year = fields.year ?: fields.publicationYear 

        session.similarResults = similarResults.collect { it.id.toString() }
        session.selectedResults = selectedResults.collect { it.id.toString() }
    }

    private List getResults(List ids) {
        ids.collect { SearchResult.get(it) }
    }

    def analyse = {
        SearchResult searchResult = SearchResult.get(params.id)

        if (searchResult.study) {
            redirect(action: view, id: searchResult.study.id)
            return
        }

        List studies = studyService.getSimilarResults(searchResult.title)
        [searchResult: searchResult, studies: studies]
    }

    def associate = {
        Study study = Study.get(params.id)
        SearchResult searchResult = SearchResult.get(params.searchResult)

        List similarResults = searchResultService.getSimilarResults(searchResult)
        List selectedResults = (study.results as List) + [searchResult]
        initSession(study.properties, 'update', similarResults, selectedResults)

        redirect(action: edit)
    }

    def newStudy = {
        SearchResult searchResult = SearchResult.get(params.id)

        List similarResults = searchResultService.getSimilarResults(searchResult)
        initSession(searchResult.properties, 'save', similarResults, [searchResult])

        redirect(action: edit)
    }

    def edit = {
        Study study = new Study(session.fields, [])
        study.results = getResults(session.selectedResults)
        [study: study, fields: session.fields, similarResults: getResults(session.similarResults)]
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

        assert !study.results.empty
        study.save(flush: true)

        redirect(action: view, id: study.id)
    }

    def update = {
        Study study = Study.get(params.id)
        println params
        println study
        study.properties = params
        study.citationCount = params.citationCount.toInteger()
      
        study.results.removeAll(getResults(session.similarResults))
        getResults(session.selectedResults).each {
            study.addResult(it)
        }

        assert !study.results.empty
        study.save(flush: true)

        redirect(action: view, id: study.id)
    }

    def view = {
        Study study = Study.get(params.id)

        assert !study.results.empty
        
        List similarResults = searchResultService.getSimilarResults(study.results.iterator().next())
        initSession(study.properties, 'update', similarResults, study.results as List)

        render(model: [study: study, fields: session.fields, similarResults: getResults(session.similarResults)], view: 'edit')
    }
}