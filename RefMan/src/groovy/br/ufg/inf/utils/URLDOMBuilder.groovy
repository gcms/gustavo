package br.ufg.inf.utils

import org.apache.http.HttpResponse
import org.apache.http.client.HttpClient
import org.apache.http.client.methods.HttpGet
import org.apache.http.impl.client.DefaultHttpClient
import org.apache.log4j.Logger
import org.w3c.dom.Document

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 28/04/2010
 * Time: 10:14:47
 * To change this template use File | Settings | File Templates.
 */
class URLDOMBuilder {
    private static Logger log = Logger.getLogger(URLDOMBuilder)

    HttpClient client
    HTMLParser parser

    public URLDOMBuilder() {
        client = new DefaultHttpClient()
        parser = new HTMLParser()
    }

    Document getDocument(URI uri) {
        log.debug "Fetching URI '${uri}'"
        Reader reader = fetchURI(uri)

        log.debug "Parsing HTML response"
        parser.parse(reader)
    }

    Reader fetchURI(URI uri) {
        HttpGet get = new HttpGet(uri)
        HttpResponse response = client.execute(get)

        String charset = response.entity.contentEncoding?.value ?: System.getProperty('file.encoding')
        new InputStreamReader(response.entity.content, charset)
    }
}
