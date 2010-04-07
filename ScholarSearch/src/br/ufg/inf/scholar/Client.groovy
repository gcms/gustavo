package br.ufg.inf.references

import org.apache.http.client.HttpClient
import org.apache.http.impl.client.DefaultHttpClient
import org.apache.http.client.methods.HttpGet
import org.apache.http.HttpResponse
import org.xml.sax.InputSource
import org.lobobrowser.html.parser.InputSourceImpl
import br.ufg.inf.references.google.GoogleScholarPageParser
import br.ufg.inf.references.google.GoogleScholarResultParser

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:57:47
 * To change this template use File | Settings | File Templates.
 */
class Client {
    HttpClient client
    PageProcessor pageProcessor

    public Client() {
        client = new DefaultHttpClient()
        pageProcessor = new PageProcessor(new GoogleScholarPageParser(), new GoogleScholarResultParser())
    }

    public List getResults(String url) {
        getResults(new HttpGet(url))
    }

    public List getResults(HttpGet get) {
        HttpResponse response = client.execute(get)
        String charset = response.entity.contentEncoding?.value ?: System.getProperty('file.encoding')
        InputSource is = new InputSourceImpl(response.entity.content, get.URI.toString(), charset)

        pageProcessor.parsePage(is)
    }

    public List executeQuery(String query) {
        List result = []
        int num = 100
        int start = 0
        while (true) {
            URI uri = new URI("http://scholar.google.com/scholar?hl=en&as_subj=eng&q=${URLEncoder.encode(query)}&num=${num}&start=${start}")
            HttpGet get = new HttpGet(uri) ///scholar?q=allintitle:+%22model-driven%22&hl=en&as_subj=eng&start=10&

            List itens = getResults(get)

            itens.each { println "${it.title} (${it.links.qtd})" }
            result.addAll(itens)
            if (itens.size() < num)
                break

            start += num
        }

        result
    }
}