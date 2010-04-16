package br.ufg.inf.utils.url

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 15/04/2010
 * Time: 08:29:49
 * To change this template use File | Settings | File Templates.
 */
class QueryURITests extends GroovyTestCase {
    public void testQuery() {
        URI uri = new URI('http://www.google.com/search?q=hello%20world')
        println uri
        assertEquals 'q=hello world', uri.query

        QueryURI query = new QueryURI(uri)
        assertEquals 'hello world', query.getQueryParam('q')

        query.setQueryParam('q', 'model driven')
        assertEquals 'model driven', query.getQueryParam('q')        
    }
}
