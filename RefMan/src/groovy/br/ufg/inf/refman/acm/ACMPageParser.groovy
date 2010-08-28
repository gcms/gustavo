package br.ufg.inf.refman.acm

import br.ufg.inf.refman.PageParser
import javax.xml.xpath.XPath
import javax.xml.xpath.XPathConstants
import javax.xml.xpath.XPathFactory
import org.apache.log4j.Logger
import org.w3c.dom.Document
import org.w3c.dom.NodeList

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:45:52
 * To change this template use File | Settings | File Templates.
 */
class ACMPageParser implements PageParser {
    private static Logger log = Logger.getLogger(ACMPageParser)

    private XPath xpath = XPathFactory.newInstance().newXPath()

    NodeList getResults(Document document) {
        log.info "Obtaining results..."
//        log.debug br.ufg.inf.utils.XMLUtils.instance.nodeToHTML(document)
        xpath.evaluate("//body/td/table/tr[@valign='top']", document, XPathConstants.NODESET)
    }
}
