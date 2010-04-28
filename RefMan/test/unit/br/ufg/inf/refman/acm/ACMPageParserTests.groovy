package br.ufg.inf.refman.acm

import javax.xml.parsers.DocumentBuilder
import javax.xml.parsers.DocumentBuilderFactory
import org.w3c.dom.Document
import br.ufg.inf.refman.PageProcessor
import br.ufg.inf.refman.SearchResult

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 16:44:15
 * To change this template use File | Settings | File Templates.
 */
class ACMPageParserTests extends GroovyTestCase {
    void testeGeral() {
        DocumentBuilder dbi = DocumentBuilderFactory.newInstance().newDocumentBuilder()
        Document doc = dbi.parse(new FileInputStream('res/acm-firstpage.xml'))

        PageProcessor pp = new PageProcessor(new ACMPageParser(), new ACMResultParser())

        List results = pp.parsePage(doc)

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
        DocumentBuilder dbi = DocumentBuilderFactory.newInstance().newDocumentBuilder()
        Document doc = dbi.parse(new FileInputStream('res/acm-author-nolink.xml'))

        PageProcessor pp = new PageProcessor(new ACMPageParser(), new ACMResultParser())

        List results = pp.parsePage(doc)

        SearchResult secureXML = results.first()

        assertFalse secureXML.authors.empty
        assertTrue secureXML.authors.contains('Belén Vela')

        SearchResult second = results[1]
        assertEquals 4, second.authors.size()
        assertTrue second.authors.contains('Belén Vela')
        assertTrue second.authors.contains('Mario Piattini')
    }
}
