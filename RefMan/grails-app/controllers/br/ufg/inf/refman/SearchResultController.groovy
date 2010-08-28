package br.ufg.inf.refman

import br.ufg.inf.utils.ObjectFilter

class SearchResultController {

    def index = { }

    def list = {
        params.max = params.max ?: 100

        List results = SearchResult.list().findAll { it.study == null }
        [results: ObjectFilter.filter(results, params), resultsCount: results.size()]
    }
}
