package br.ufg.inf.scholar

import org.apache.http.client.HttpClient
import org.apache.http.impl.client.DefaultHttpClient
import org.apache.http.client.methods.HttpGet
import org.apache.http.HttpResponse
import org.xml.sax.InputSource
import org.lobobrowser.html.parser.InputSourceImpl


/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:57:47
 * To change this template use File | Settings | File Templates.
 */
class Client {
    HttpClient client
    PageParser pageParser

    public Client() {
        client = new DefaultHttpClient()
        pageParser = new PageParser()
    }

    public List getResults(String url) {
        HttpGet get = new HttpGet(url)
        getResults(get)
    }

    public List getResults(HttpGet get) {
        HttpResponse response = client.execute(get)
        String charset = response.entity.contentEncoding?.value ?: System.getProperty('file.encoding')
        InputSource is = new InputSourceImpl(response.entity.content, 'http://scholar.google.com/', charset)

        pageParser.parsePage(is)
    }

    public List executeQuery(String query) {
        List result = []
        int num = 100
        int start = 0
        while (true) {
            URI uri = new URI("http://scholar.google.com/scholar?hl=en&as_subj=eng&q=${query}&num=${num}&start=${start}")
            HttpGet get = new HttpGet(uri) ///scholar?q=allintitle:+%22model-driven%22&hl=en&as_subj=eng&start=10&

            List itens = getResults(get)

            itens.each { println "${it.title} (${it.links.qtd})" }
            result.addAll(itens)
            if (itens.size() < 10)
                break

            start += num
        }

        result
    }
}
