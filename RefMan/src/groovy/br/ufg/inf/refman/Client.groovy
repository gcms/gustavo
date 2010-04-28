package br.ufg.inf.refman

import org.apache.http.client.HttpClient
import org.apache.http.impl.client.DefaultHttpClient
import org.apache.http.client.methods.HttpGet
import org.apache.http.HttpResponse
import org.xml.sax.InputSource
import org.lobobrowser.html.parser.InputSourceImpl

import org.lobobrowser.html.UserAgentContext
import javax.xml.parsers.DocumentBuilder
import org.lobobrowser.html.parser.DocumentBuilderImpl
import org.lobobrowser.html.test.SimpleUserAgentContext
import org.w3c.dom.Document
import br.ufg.inf.refman.query.URLBuilder
import org.apache.log4j.Logger
import br.ufg.inf.utils.URLDOMBuilder

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:57:47
 * To change this template use File | Settings | File Templates.
 */
class Client {
    private static Logger log = Logger.getLogger(Client)

    URLDOMBuilder domBuilder

    URLBuilder queryBuilder
    PageProcessor pageProcessor
    SiteDirector siteDirector

    public Client(URLBuilder queryBuilder, SiteDirector siteDirector,
                  PageParser pageParser, ResultParser resultParser) {
        domBuilder = new URLDOMBuilder()

        this.queryBuilder = queryBuilder
        this.siteDirector = siteDirector
        pageProcessor = new PageProcessor(pageParser, resultParser)
    }

    public List getResultsFromURI(URI uri) {
        log.debug "Getting DOM from '${uri}'"
        Document document = domBuilder.getDocument(uri)
        
        log.debug "Extracting results"
        pageProcessor.parsePage(document)
    }

    public List executeURL(String url) {
        siteDirector.collectPages(url) {URI uri ->            
            getResultsFromURI(uri)
        }
    }

    public List executeQuery(String query) {
        executeURL(buildURL(query))
    }

    public String buildURL(String query) {
        queryBuilder.buildURL(query)
    }
}

