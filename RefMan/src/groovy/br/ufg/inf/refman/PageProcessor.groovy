package br.ufg.inf.refman

import org.w3c.dom.Document

import org.w3c.dom.NodeList
import org.w3c.dom.Node
import org.apache.log4j.Logger

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 12:51:55
 * To change this template use File | Settings | File Templates.
 */
class PageProcessor {
    private static Logger log = Logger.getLogger(PageProcessor)
    PageParser pageParser
    ResultParser resultParser

    public PageProcessor(PageParser pageParser, ResultParser resultParser) {

        this.pageParser = pageParser
        this.resultParser = resultParser
    }

    List parsePage(Document document) {
        NodeList resultNodes = pageParser.getResults(document)
        log.debug "Nodes obtained: ${resultNodes.length}"

        resultNodes.collect {Node node ->
            resultParser.parse(node)
        }        
    }
}