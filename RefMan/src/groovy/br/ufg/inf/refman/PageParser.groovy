package br.ufg.inf.refman

import org.w3c.dom.NodeList
import org.w3c.dom.Document

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 16:08:16
 * To change this template use File | Settings | File Templates.
 */
interface PageParser {
    NodeList getResults(Document document)
}
