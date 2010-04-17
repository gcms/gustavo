package br.ufg.inf.refman.query

import br.ufg.inf.refman.Engine

class QueryService {

    static transactional = true
    static scope = 'session'

    private String lastKey
    private List lastResults

    List getResults(Engine engine, String url) {
        String key = getKey(engine, url)

        if (key == lastKey)
            return lastResults

        lastKey = key
        lastResults = engine.client.executeURL(url)
    }

    private String getKey(Engine engine, String query) {
        "${engine.id}#${query}"
    }
}
