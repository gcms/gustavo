package br.ufg.inf.utils

import org.w3c.dom.Document

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 28/04/2010
 * Time: 10:28:07
 * To change this template use File | Settings | File Templates.
 */
class DOMExtractApp {
    static void main(String[] args) {
        String url = "http://portal.acm.org/results.cfm?within=&CFID=88256700&CFTOKEN=47174228&adv=1&COLL=ACM&qrycnt=47&DL=ACM&Go.x=39&Go.y=12&termzone=Title&allofem=Model+driven+development+secure+XML&anyofem=&noneofem=&peoplezone=Name&people=&peoplehow=and&keyword=&keywordhow=AND&affil=&affilhow=AND&pubin=&pubinhow=and&pubby=&pubbyhow=OR&since_year=&before_year=&pubashow=OR&sponsor=&sponsorhow=AND&confdate=&confdatehow=OR&confloc=&conflochow=OR&isbnhow=OR&isbn=&doi=&ccs=&subj="

        Document document = new URLDOMBuilder().getDocument(new URI(url))
        println XMLUtils.instance.nodeToHTML(document)
    }
}
