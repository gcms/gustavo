package br.ufg.inf.refman

import br.ufg.inf.utils.LevenshteinDistance
import br.ufg.inf.utils.Normalizer

class StudyService {

    static transactional = true
    static scope = 'singleton'

    int relevanceThreshold = 10

    List getSimilarResults(String title) {
        List allResults = Study.list()

        String target = Normalizer.normalize(title)
        List distances = allResults.collect { Study it ->
            String source = Normalizer.normalize(it.title)
            [result: it, distance: LevenshteinDistance.calculateDistanceToLengthRation(source, target)]
        }

        filterRelevantResults(distances.sort { it.distance }.collect { it.result })
    }

    private List filterRelevantResults(List results) {
        results.size() < relevanceThreshold ? results : results[0..<relevanceThreshold]
    }
}