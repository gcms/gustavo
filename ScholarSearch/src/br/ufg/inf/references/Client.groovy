package br.ufg.inf.references

import org.apache.http.client.HttpClient
import org.apache.http.impl.client.DefaultHttpClient
import org.apache.http.client.methods.HttpGet
import org.apache.http.HttpResponse
import org.xml.sax.InputSource
import org.lobobrowser.html.parser.InputSourceImpl
import br.ufg.inf.references.google.GoogleScholarPageParser
import br.ufg.inf.references.google.GoogleScholarResultParser
import br.ufg.inf.references.google.GoogleScholarSiteDirector
import org.apache.log4j.Logger

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:57:47
 * To change this template use File | Settings | File Templates.
 */
class Client {
    private static Logger log = Logger.getLogger(Client)

    HttpClient client
    PageProcessor pageProcessor
    SiteDirector siteDirector

    public Client(SiteDirector siteDirector, PageParser pageParser, ResultParser resultParser) {
        client = new DefaultHttpClient()
        this.siteDirector = siteDirector
        pageProcessor = new PageProcessor(pageParser, resultParser)
    }

    public Client() {
        this(new GoogleScholarSiteDirector(), new GoogleScholarPageParser(), new GoogleScholarResultParser())
    }

    private InputSource executeRequest(URI uri) {
        HttpGet get = new HttpGet(uri)
        HttpResponse response = client.execute(get)

        String charset = response.entity.contentEncoding?.value ?: System.getProperty('file.encoding')
        new InputSourceImpl(response.entity.content, get.URI.toString(), charset)
    }

    public List getResultsFromURI(URI uri) {
        InputSource response = executeRequest(uri)
        pageProcessor.parsePage(response)
    }

    public List executeQuery(String query) {
        siteDirector.collectPages(query) { URI uri ->
            println "Getting results from '${uri}'"
            getResultsFromURI(uri)
        }
    }
}