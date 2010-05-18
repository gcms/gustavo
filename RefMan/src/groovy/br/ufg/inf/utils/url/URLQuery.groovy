package br.ufg.inf.utils.url

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 14/04/2010
 * Time: 13:31:09
 * To change this template use File | Settings | File Templates.
 */
class URLQuery {
    private List entries = []

    public URLQuery(String query) {
        query.tokenize('&?').each { String keyValStr ->
            String[] keyVal = keyValStr.split('=', 2)
            entries.add(new MapEntry(keyVal[0], keyVal[1]))
        }
    }

    private MapEntry getEntry(String key) {
        MapEntry entry = entries.find { it.key == key }
        if (entry == null) {
            entry = new MapEntry(key, null)
            entries.add(entry)
        }

        entry
    }

    public void setParam(String key, String val) {
        getEntry(key).value = val
    }

    public String getParam(String key) {
        getEntry(key).value
    }

    public String toString() {
        entries.findAll { it.value }.collect { "${it.key}=${it.value}" }.join('&')
    }
}
