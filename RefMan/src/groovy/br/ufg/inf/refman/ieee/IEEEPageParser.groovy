package br.ufg.inf.refman.ieee

import br.ufg.inf.refman.PageParser
import javax.xml.xpath.XPath
import javax.xml.xpath.XPathConstants
import javax.xml.xpath.XPathFactory
import org.w3c.dom.Document
import org.w3c.dom.NodeList
import br.ufg.inf.utils.XMLUtils

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:45:52
 * To change this template use File | Settings | File Templates.
 */

class IEEEPageParser implements PageParser {
    private XPath xpath = XPathFactory.newInstance().newXPath()

    NodeList getResults(Document document) {
//        def file = new File('res/ieee.xml')
//        file.text = XMLUtils.instance.nodeToHTML(xpath.evaluate('//body', document, XPathConstants.NODE))
//        NodeList nodes = xpath.evaluate("//li[@class='noAbstract']", document, XPathConstants.NODESET)
//        println nodes.length
//        nodes.each {
//            println 'ok "' + xpath.evaluate("div[@class='header']//a", it) + '"'
//        }
        xpath.evaluate("//ul[@class='Results']/li[@class='noAbstract']", document, XPathConstants.NODESET)
    }
}