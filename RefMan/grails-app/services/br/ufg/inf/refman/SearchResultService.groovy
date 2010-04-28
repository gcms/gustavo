package br.ufg.inf.refman

class SearchResultService {

    static transactional = true
    static scope = 'singleton'

    int relevanceThreshold = 10

    List getSimilarResults(SearchResult searchResult) {
        List allResults = SearchResult.list().findAll { it.study == null } - searchResult

        log.debug "Results found: ${allResults.size()}"

        List distances = allResults.collect { SearchResult it ->
            [result: it, distance: searchResult.calculateDistanceToLengthRation(it)]
        }

        filterRelevantResults(distances.sort { it.distance }.collect { it.result })
    }

    private List filterRelevantResults(List results) {
        results.size() < relevanceThreshold ? results : results[0..<relevanceThreshold]  
    }
}
