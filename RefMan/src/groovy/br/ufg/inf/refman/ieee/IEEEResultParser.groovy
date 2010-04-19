package br.ufg.inf.refman.ieee

import br.ufg.inf.refman.ResultParser
import br.ufg.inf.refman.SearchResult
import org.w3c.dom.Node
import javax.xml.xpath.XPath
import javax.xml.xpath.XPathFactory
import br.ufg.inf.utils.XMLUtils
import javax.xml.xpath.XPathConstants
import br.ufg.inf.refman.Publication
import org.apache.log4j.Logger

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 17/04/2010
 * Time: 12:22:45
 * To change this template use File | Settings | File Templates.
 */
class IEEEResultParser implements ResultParser {
    private static Logger log = Logger.getLogger(IEEEResultParser)

    private XPath xpath = XPathFactory.newInstance().newXPath()
    
    public SearchResult parse(Node node) {
        println XMLUtils.instance.nodeToHTML(node)
        new SearchResult(html: XMLUtils.instance.nodeToHTML(node),
                title: parseTitle(node), authors: parseAuthors(node),
                description: parseDescription(node), publication: parsePublication(node))
    }

    private String parseDescription(Node node) {
        xpath.evaluate("div[@class='abstract RevealContent']", node).trim()
    }

    private String parseTitle(Node node) {
        xpath.evaluate("div[@class='header']//a", node).replaceAll('\\s+', ' ').trim()
    }

    private String getAuthorsLine(Node node) {
        Node p = xpath.evaluate("div[@class='header']//div[@class='detail']/p", node, XPathConstants.NODE)

        p.firstChild.textContent.trim()
    }

    private List parseAuthors(Node node) {
        getAuthorsLine(node).tokenize(';').collect { String author ->
            author.tokenize(',').collect { it.trim() }.reverse().join(' ').trim()
        }.findAll { it.trim().length() > 0 }
    }

    private Publication parsePublication(Node node) {
        String line = xpath.evaluate("div[@class='header']//div[@class='detail']/p/a", node).tokenize('.').first().trim()
        //println "YearPub: ${line}"

        int comma = line.lastIndexOf(',')
        if (comma == -1)
            return new Publication(name: line)
                
        String name = line.substring(0, comma).trim()
        Integer year = line.substring(comma + 1).find('\\d{4}')?.toInteger()
        new Publication(name: name, year: year)
    }
}
