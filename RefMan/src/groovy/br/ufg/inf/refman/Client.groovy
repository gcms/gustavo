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

    UserAgentContext context
    DocumentBuilder dbi

    URLBuilder queryBuilder
    PageProcessor pageProcessor
    SiteDirector siteDirector

    public Client(URLBuilder queryBuilder, SiteDirector siteDirector,
                  PageParser pageParser, ResultParser resultParser) {
        client = new DefaultHttpClient()
        context = new ScriptSetupSimpleUserAgentContext() //new SimpleUserAgentContext()
        dbi = new DocumentBuilderImpl(context)

        this.queryBuilder = queryBuilder
        this.siteDirector = siteDirector
        pageProcessor = new PageProcessor(pageParser, resultParser)
    }

    private InputSource executeRequest(URI uri) {
        HttpGet get = new HttpGet(uri)
        HttpResponse response = client.execute(get)

        String charset = response.entity.contentEncoding?.value ?: System.getProperty('file.encoding')
        new InputSourceImpl(response.entity.content, get.URI.toString(), charset)
    }

    public List getResultsFromURI(URI uri) {
        log.debug "Getting results from '${uri}'..."
        InputSource response = executeRequest(uri)

        log.debug "Parsing HTML..."
        Document document = dbi.parse(response)

        log.debug "Extracting data..."
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


class ScriptSetupSimpleUserAgentContext extends SimpleUserAgentContext {
    boolean scriptEnabled = false
    
    public boolean isScriptingEnabled() {
        scriptEnabled
    }
}
