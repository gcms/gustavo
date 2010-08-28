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
        URI uri = new URI('http://www.google.com/search?q=hello%20world&mode=123')
        println uri
        assertEquals 'q=hello world&mode=123', uri.query

        QueryURI query = new QueryURI(uri)
        assertEquals 'hello world', query.getQueryParam('q')

        query.setQueryParam('q', 'model driven')
        assertEquals 'model driven', query.getQueryParam('q')        
    }

    public void testMultipleFields() {
        URI uri = new URI('http://www.google.com/search?q=hello%20world&q=2')
        assertEquals 'q=hello world&q=2', uri.query

        QueryURI query = new QueryURI(uri)
        assertEquals 'hello world', query.getQueryParam('q')

        query.setQueryParam('q', 'model driven')
        assertEquals 'model driven', query.getQueryParam('q')

        assertEquals 'q=model driven&q=2', query.URI.query
    }

    public void testACM() {
        String url = "http://portal.acm.org/results.cfm?within=&CFID=96422564&CFTOKEN=88074955&adv=1&COLL=Portal&qrycnt=3&DL=ACM&Go.x=3&Go.y=10&termzone=all&allofem=&anyofem=&noneofem=&peoplezone=Name&people=&peoplehow=and&keyword=%22model+driven%22&keywordhow=AND&affil=&affilhow=AND&pubin=&pubinhow=and&pubby=&pubbyhow=OR&since_year=&before_year=&pubashow=OR&sponsor=&sponsorhow=AND&confdate=&confdatehow=OR&confloc=&conflochow=OR&isbnhow=OR&isbn=&doi=&ccs=&subj="

        assertEquals url, new URI(url).toString()
        assertEquals url, new URL(url).toString()


        QueryURI queryURI = new QueryURI(url)
        assertEquals url, queryURI.toString()

        queryURI.setQueryParam('start', 1)
        assertEquals(url + "&start=1", queryURI.toString())

    }
}
