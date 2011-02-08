package br.ufg.inf.refman.acm

import javax.xml.parsers.DocumentBuilder
import javax.xml.parsers.DocumentBuilderFactory
import org.w3c.dom.Document
import br.ufg.inf.refman.PageProcessor
import br.ufg.inf.refman.SearchResult
import br.ufg.inf.utils.URLDOMBuilder
import br.ufg.inf.utils.XMLUtils
import com.sun.org.apache.xml.internal.serialize.OutputFormat

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 16:44:15
 * To change this template use File | Settings | File Templates.
 */
class ACMPageParserTests extends GroovyTestCase {

    private List parseDOM(String filename) {
        DocumentBuilder dbi = DocumentBuilderFactory.newInstance().newDocumentBuilder()
        Document doc = dbi.parse(new FileInputStream(filename))

        PageProcessor pp = new PageProcessor(new ACMPageParser(), new ACMResultParser())

        pp.parsePage(doc)
    }

    void testeGeral() {
        List results = parseDOM('res/acm-firstpage.xml')

        assertEquals 20, results.size()

        SearchResult first = results.first()
        assertEquals 'Colored graph transformation rules for model-driven engineering of multi-target systems', first.title
        assertTrue first.authors.contains('Adrian Stanciulescu')
        assertEquals 3, first.authors.size()
        assertEquals 2008, first.publication.year
        assertTrue first.publication.name.contains('Proceedings of the third international workshop on Graph and model transformations')
        assertTrue first.description.startsWith('Multi-target systems are')
        assertEquals 0, first.citationCount

        assertTrue results.every { it.citationCount >= 0 }
    }

    void testeAuthorNoLink() {
        List results = parseDOM('res/acm-author-nolink.xml')

        SearchResult secureXML = results.first()

        assertFalse secureXML.authors.empty
        assertTrue secureXML.authors.contains('Belén Vela')

        SearchResult second = results[1]
        assertEquals 4, second.authors.size()
        assertTrue second.authors.contains('Belén Vela')
        assertTrue second.authors.contains('Mario Piattini')
    }

    void testeNovoACMFirstPage() {
        String url = "http://portal.acm.org/results.cfm?within=&CFID=96422564&CFTOKEN=88074955&adv=1&COLL=Portal&qrycnt=3&DL=ACM&Go.x=3&Go.y=10&termzone=all&allofem=&anyofem=&noneofem=&peoplezone=Name&people=&peoplehow=and&keyword=%22model+driven%22&keywordhow=AND&affil=&affilhow=AND&pubin=&pubinhow=and&pubby=&pubbyhow=OR&since_year=&before_year=&pubashow=OR&sponsor=&sponsorhow=AND&confdate=&confdatehow=OR&confloc=&conflochow=OR&isbnhow=OR&isbn=&doi=&ccs=&subj="

        Document document = new URLDOMBuilder().getDocument(new URI(url))
        PageProcessor pp = new PageProcessor(new ACMPageParser(), new ACMResultParser())

        List results = pp.parsePage(document) //parseDOM('res/acm-firstpage-new.xml')

        assertEquals 20, results.size()

        results.each { SearchResult result ->
            assertTrue result.title && result.title.trim() != ''
            assertTrue result.authors && result.authors.size() > 0
        }

        SearchResult first = results.first()
        assertEquals 'Modeling and Analysis of Advanced Automotive ECU Architectures at Early Design Stages Using EMF and Model Transformation', first.title
        assertTrue first.authors.contains('Christoph Lauer')
        assertTrue first.authors.contains('Jens Pollmer')
        assertEquals 3, first.authors.size()
        
        assertEquals 2010, first.publication.year
        assertTrue first.publication.name.contains('Proceedings of the 2010 Fourth International Conference on Secure Software')
        assertNotNull first.description
        assertTrue first.description.startsWith('In the automotive')
        assertEquals 0, first.citationCount

        assertTrue results.every { it.citationCount >= 0 }

        SearchResult second = results[1]
        assertNotNull second.description
        assertTrue second.description.startsWith('Programming sensor/actuator')

        SearchResult last = results.last()
        assertEquals 'An Approach for Evolving Transformation Chains', last.title
        assertTrue last.description.startsWith('A transformation chain')
        
    }
}
