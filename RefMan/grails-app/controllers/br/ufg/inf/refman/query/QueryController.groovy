package br.ufg.inf.refman.query

import br.ufg.inf.refman.Engine
import br.ufg.inf.refman.QueryResult
import br.ufg.inf.utils.ObjectFilter

class QueryController {
    QueryService queryService

    static allowedMethods = [
            index: 'GET', query: 'POST', results: 'GET',
            save: 'POST', list: 'GET', view: 'GET'
    ]

    def index = {
        redirect(action: create)
    }

    def create = {
        [engines: Engine.list()]
    }

    def query = {
        Engine engine = Engine.get(params.engine)

        session.url = params.url ?: engine.client.buildURL(params.query)
        session.results = queryService.getResults(engine, session.url)
        session.engine = engine.id
        redirect(action: results)
    }

    def results = {
        if (!session.results || session.results.empty || !session.engine) {
            flash.message = 'No results found'
            redirect(action: create)
            return
        }

        QueryResult query = new QueryResult(Engine.get(session.engine), session.results, '', '', session.url)        
        [query: query]
    }


    def update = {
        QueryResult queryResult = QueryResult.get(params.id)
        queryResult.properties = params
        queryResult.save(flush: true)
        redirect(action: view, id: params.id)
    }

    
    def save = {
        Engine engine = Engine.get(params.engine)
        QueryResult queryResult = new QueryResult(engine, session.results, params.name, params.description, params.url)

        assert queryResult.results.size() > 0

        queryResult.save(flush: true)
        redirect(action: list)
    }

    def list = {
        params.max = params.max ?: 100
        [queries: ObjectFilter.filter(QueryResult.list(), params), queriesCount: QueryResult.count()]
    }

    def delete = {
        QueryResult queryResult = QueryResult.get(params.id)
        queryResult.delete(flush: true)
        redirect(action: list)
    }

    def view = {
        QueryResult queryResult = QueryResult.get(params.id)

        [query: queryResult]
    }
}