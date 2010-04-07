package br.ufg.inf.scholar

import javax.xml.xpath.XPath
import javax.xml.xpath.XPathFactory
import org.w3c.dom.Node
import org.w3c.dom.NodeList
import javax.xml.xpath.XPathConstants

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 11:02:32
 * To change this template use File | Settings | File Templates.
 */
class ResultParser {
    private XPath xpath

    public ResultParser() {
        xpath = XPathFactory.newInstance().newXPath()
    }

    public Result parse(Node node) {
        new Result(type: parseType(node), title: parseTitle(node), url: parseURL(node),
                authors: parseAuthors(node), publication: parsePublication(node),
                description: parseDescription(node), links: parseLinks(node))
    }

    public String parseType(Node node) {
        xpath.evaluate("h3/span[@class='gs_ctc']", node) ?:
            xpath.evaluate("h3/span[@class='gs_ctu']", node) ?: ''
    }

    public String parseTitle(Node node) {
        xpath.evaluate("h3/a", node) ?: parseTitleFromContent(node)
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
        InformationBuilder builder = new InformationBuilder()
        builder.parseAuthorLine(getAuthorLine(node))
        builder.authors
    }

    private Publication parsePublication(Node node) {
        InformationBuilder builder = new InformationBuilder()
        builder.parseAuthorLine(getAuthorLine(node))
        builder.publication
    }

    private Links parseLinks(Node node) {
        NodeList nodes = xpath.evaluate("font/span[@class='gs_fl']/a", node, XPathConstants.NODESET)

        Links links = new Links()

        for (int i = 0; i < nodes.length; i++) {
            Node item = nodes.item(i)
            def m = (item.textContent =~ ~/.*Cited by (\d+).*/)
            if (m.matches()) {
                links.qtd = m.group(1).toInteger()
                links.citedBy = xpath.evaluate('@href', item)
            } else if (item.textContent =~ ~/.*Related articles.*/) {
                links.related = xpath.evaluate('@href', item)
            } else if (item.textContent =~ ~/.*All.*versions.*/) {
                links.versions = xpath.evaluate('@href', item)
            }
        }

        links
    }

    private String parseDescription(Node node) {
        Node font = xpath.evaluate("font", node, XPathConstants.NODE)
        Node authorsLine = xpath.evaluate("font/span[@class='gs_a']", node, XPathConstants.NODE)
        Node linksLine = xpath.evaluate("font/span[@class='gs_fl']", node, XPathConstants.NODE)

        font.removeChild(authorsLine)
        font.removeChild(linksLine)

        font.textContent
    }

}
