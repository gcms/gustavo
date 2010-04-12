package br.ufg.inf.refman.acm

import br.ufg.inf.refman.ResultParser
import br.ufg.inf.refman.SearchResult
import br.ufg.inf.utils.XMLUtils
import org.w3c.dom.Node
import org.w3c.dom.NodeList
import javax.xml.xpath.XPathFactory
import javax.xml.xpath.XPath
import javax.xml.xpath.XPathConstants
import br.ufg.inf.refman.Publication

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:40:45
 * To change this template use File | Settings | File Templates.
 */
class ACMResultParser implements ResultParser {
    XPath xpath = XPathFactory.newInstance().newXPath()
    SearchResult parse(Node node) {
        new SearchResult(html: XMLUtils.instance.nodeToHTML(node),
                title: parseTitle(node), description: parseDescription(node),
                authors: parseAuthors(node), publication: parsePublication(node),
                citationCount: parseCitationCount(node))
    }

    public String parseTitle(Node node) {
        xpath.evaluate("td[2]//A", node).trim()
    }

    private String parseDescription(Node node) {
        xpath.evaluate("td[2]//div[@class='abstract2']//p", node).trim()
    }

    public List parseAuthors(Node node) {
        NodeList list = xpath.evaluate("td[2]//div[@class='authors']//a", node, XPathConstants.NODESET)

        List result = []
        list.each { Node author -> result.add(author.textContent.trim()) }
        result
    }

    public Publication parsePublication(Node node) {
        new Publication(year: parseYear(node), name: parseName(node))
    }

    private Integer parseYear(Node node) {
        String dateLine = xpath.evaluate("td[2]//col//td[@class='small-text']", node)
        String year = dateLine.replaceAll('\\D', '')
        year.isInteger() ? year.toInteger() : null
    }

    private String parseName(Node node) {
        xpath.evaluate("td[2]//col//div[@class='addinfo']", node).replaceAll('\\s+', ' ').trim()
    }

    private int parseCitationCount(Node node) {
        def m = node.textContent =~ ~/Citation Count: (\d+)/
        if (m.find()) {
            return m.group(1).toInteger()
        }
        return -1
    }
}
