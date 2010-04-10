package br.ufg.inf.references.google

import javax.xml.xpath.XPath
import javax.xml.xpath.XPathFactory
import org.w3c.dom.Node
import org.w3c.dom.NodeList
import javax.xml.xpath.XPathConstants
import br.ufg.inf.references.SearchResult

import br.ufg.inf.references.Publication
import br.ufg.inf.references.Link
import br.ufg.inf.references.ResultParser
import org.apache.log4j.Logger

import br.ufg.inf.references.Utils

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 11:02:32
 * To change this template use File | Settings | File Templates.
 */
class GoogleScholarResultParser implements ResultParser {
    private static Logger log = Logger.getLogger(GoogleScholarResultParser)
    private XPath xpath = XPathFactory.newInstance().newXPath()

    public SearchResult parse(Node node) {
        new SearchResult(html: getHTML(node),
                type: parseType(node), title: parseTitle(node), url: parseURL(node),
                authors: parseAuthors(node), publication: parsePublication(node),
                description: parseDescription(node), links: parseLinks(node))
    }

    public String getHTML(Node node) {
        Utils.instance.nodeToHTML(node)
     }

    public String parseType(Node node) {
            xpath.evaluate("h3/span[@class='gs_ctc']", node) ?:
                xpath.evaluate("h3/span[@class='gs_ctu']", node) ?: ''
    }

    public String parseTitle(Node node) {
        String title = xpath.evaluate("h3/a", node) ?: parseTitleFromContent(node)
        println "Title: ${title}"
        title
    }

    private String parseTitleFromContent(Node node) {
        Node h3 = xpath.evaluate("h3", node, XPathConstants.NODE)
        Node span = xpath.evaluate("h3/span[@class='gs_ctu']", node, XPathConstants.NODE)

        h3.removeChild(span)
        h3.textContent
    }

    private String parseURL(Node node) {
        xpath.evaluate("h3/a/@href", node)
    }

    private String getAuthorLine(Node node) {
        xpath.evaluate("font/span[@class='gs_a']", node)
    }

    private List parseAuthors(Node node) {
        AuthorPublicationBuilder builder = new AuthorPublicationBuilder()
        builder.parseAuthorLine(getAuthorLine(node))
        builder.authors
    }

    private Publication parsePublication(Node node) {
        AuthorPublicationBuilder builder = new AuthorPublicationBuilder()
        builder.parseAuthorLine(getAuthorLine(node))
        builder.publication
    }

    private List parseLinks(Node node) {
        NodeList nodes = xpath.evaluate("font/span[@class='gs_fl']/a", node, XPathConstants.NODESET)

        List links = []

        for (int i = 0; i < nodes.length; i++) {
            Node item = nodes.item(i)
            links << new Link(label: item.textContent, url: xpath.evaluate('@href', item))
        }

        links
    }

    private String parseDescription(Node node) {
        Node font = xpath.evaluate("font", node, XPathConstants.NODE)
        Node authorsLine = xpath.evaluate("font/span[@class='gs_a']", node, XPathConstants.NODE)
        Node linksLine = xpath.evaluate("font/span[@class='gs_fl']", node, XPathConstants.NODE)

        println "Authors: ${authorsLine?.textContent}"
        if (authorsLine)
            font.removeChild(authorsLine)

        println "Links :${linksLine?.textContent}"
        if (linksLine)
            font.removeChild(linksLine)

        font.textContent.replaceAll('\\s+', ' ').trim()
    }
}
