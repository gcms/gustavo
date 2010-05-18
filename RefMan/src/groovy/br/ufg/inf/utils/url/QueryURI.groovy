package br.ufg.inf.utils.url

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 14/04/2010
 * Time: 13:29:44
 * To change this template use File | Settings | File Templates.
 */
class QueryURI {
    private URI uri
    private URLQuery query

    public QueryURI(String uri) {
        this(new URI(uri))
    }

    public QueryURI(URI uri) {
        this.uri = uri
        query = new URLQuery(uri.query)
    }

    public String getQueryParam(String key) {
        query.getParam(key)
    }

    public void setQueryParam(String key, value) {
        query.setParam(key, value.toString())
        reset()
    }

    private void reset() {
        uri = new URI(uri.scheme, uri.userInfo, uri.host, uri.port, uri.path, query.toString(), uri.fragment)
    }

    public URI getURI() {
        uri
    }

    public String toString() {
        getURI().toString()
    }
}
