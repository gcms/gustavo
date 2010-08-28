package br.ufg.inf.utils

import br.ufg.inf.refman.acm.ACMPageParser
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
        String url = "http://portal.acm.org/results.cfm?within=&CFID=96422564&CFTOKEN=88074955&adv=1&COLL=Portal&qrycnt=3&DL=ACM&Go.x=3&Go.y=10&termzone=all&allofem=&anyofem=&noneofem=&peoplezone=Name&people=&peoplehow=and&keyword=%22model+driven%22&keywordhow=AND&affil=&affilhow=AND&pubin=&pubinhow=and&pubby=&pubbyhow=OR&since_year=&before_year=&pubashow=OR&sponsor=&sponsorhow=AND&confdate=&confdatehow=OR&confloc=&conflochow=OR&isbnhow=OR&isbn=&doi=&ccs=&subj="

        Document document = new URLDOMBuilder().getDocument(new URI(url))
        println XMLUtils.instance.nodeToHTML(document)

         println new ACMPageParser().getResults(document).length
    }
}
