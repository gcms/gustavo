package br.ufg.inf.utils.url

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 14/04/2010
 * Time: 13:31:09
 * To change this template use File | Settings | File Templates.
 */
class URLQuery {
    private LinkedHashMap query = [:]

    public URLQuery(String query) {
        println query
        query.tokenize('&?').each { String keyValStr ->
            String[] keyVal = keyValStr.split('=', 2)
            setParam(keyVal[0], keyVal[1])
        }
    }

    public void setParam(String key, String val) {
        query[key] = val
    }

    public String getParam(String key) {
        query[key]
    }

    public String toString() {
        query.entrySet().collect { "${it.key}=${it.value}" }.join('&')
    }
}
