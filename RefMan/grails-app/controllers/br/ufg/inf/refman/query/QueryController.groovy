package br.ufg.inf.refman.query

import br.ufg.inf.refman.Engine
import br.ufg.inf.refman.Client

class QueryController {

    def index = {
        println Engine.list().size()
        [engines: Engine.list()]
    }

    def query = {
        //validate
        Engine engine = Engine.get(params.engine)
        Client client = engine.newClient()

        [results: client.executeQuery(params.query)]
    }
}
