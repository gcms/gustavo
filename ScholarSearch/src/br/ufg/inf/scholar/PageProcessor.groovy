package br.ufg.inf.references

import org.xml.sax.InputSource
import javax.xml.xpath.XPathConstants
import javax.xml.xpath.XPath
import javax.xml.xpath.XPathFactory
import org.w3c.dom.Document
import org.lobobrowser.html.parser.DocumentBuilderImpl
import javax.xml.parsers.DocumentBuilder
import org.lobobrowser.html.test.SimpleUserAgentContext
import org.lobobrowser.html.UserAgentContext
import org.w3c.dom.NodeList
import org.w3c.dom.Node
import br.ufg.inf.references.google.GoogleScholarResultParser

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:51:55
 * To change this template use File | Settings | File Templates.
 */
class PageProcessor {
    UserAgentContext context
    DocumentBuilder dbi
    PageParser pageParser
    ResultParser resultParser

    public PageProcessor(PageParser pageParser, ResultParser resultParser) {
        context = new SimpleUserAgentContext();
        dbi = new DocumentBuilderImpl(context)
        this.pageParser = pageParser
        this.resultParser = resultParser
    }

    List parsePage(InputSource is) {
        Document document = dbi.parse(is)

        //println document.dump()

        NodeList resultNodes = pageParser.getResults(document)

        List result = []       
        resultNodes.eachWithIndex {Node node, index ->
            result.add(resultParser.parse(node))
        }        
        result
    }
}
