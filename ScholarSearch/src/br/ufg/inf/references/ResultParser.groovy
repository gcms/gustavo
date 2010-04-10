package br.ufg.inf.references

import org.w3c.dom.Node

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 16:01:02
 * To change this template use File | Settings | File Templates.
 */
interface ResultParser {
    SearchResult parse(Node node)
}
