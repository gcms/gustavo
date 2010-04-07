package br.ufg.inf.scholar

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

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:51:55
 * To change this template use File | Settings | File Templates.
 */
class PageParser {
    UserAgentContext context
    DocumentBuilder dbi
    ResultParser resultParser

    public PageParser() {
        context = new SimpleUserAgentContext();
        dbi = new DocumentBuilderImpl(context)
        resultParser = new ResultParser()
    }

    List parsePage(InputSource is) {
        Document document = dbi.parse(is)

        println document.dump()

        XPathFactory xpathFactory = XPathFactory.newInstance()
        XPath xpath = xpathFactory.newXPath()

        NodeList resultNodes = xpath.evaluate("html//div[@class='gs_r']", document, XPathConstants.NODESET)

        List result = []       
        resultNodes.eachWithIndex {Node node, index ->
            result.add(resultParser.parse(node))
        }        
        result
    }
}
