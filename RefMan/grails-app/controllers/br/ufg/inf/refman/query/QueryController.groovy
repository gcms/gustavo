package br.ufg.inf.refman.query

import br.ufg.inf.refman.Engine
import br.ufg.inf.refman.QueryResult

class QueryController {
    QueryService queryService

    def index = {
        println Engine.list().size()
        [engines: Engine.list()]
    }

    def query = {
        Engine engine = Engine.get(params.engine)

        session.results = queryService.getResults(engine, params.query)
        session.engine = engine.id
        redirect(action: results)
    }

    def results = {
        if (!session.results || session.results.empty || !session.engine) {
            redirect(action: index)
            return
        }
        
        [engine: session.engine, results: session.results]
    }

    def save = {
        Engine engine = Engine.get(params.engine)
        QueryResult queryResult = new QueryResult(engine, session.results, params.name, params.description)
        assert queryResult.results.size() > 0
        queryResult.save(flush: true)
        redirect(action: list)
    }

    def delete = {
        QueryResult queryResult = QueryResult.get(params.id)
        queryResult.delete(flush: true)
        redirect(action: list)
    }

    def list = {
        [results: QueryResult.list()]
    }

    def view = {
        QueryResult queryResult = QueryResult.get(params.id)

        [query: queryResult]
    }
}
