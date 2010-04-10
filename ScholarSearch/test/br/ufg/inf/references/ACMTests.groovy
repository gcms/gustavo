package br.ufg.inf.references

import javax.xml.parsers.DocumentBuilder
import javax.xml.parsers.DocumentBuilderFactory
import org.w3c.dom.Document

import br.ufg.inf.references.acm.ACMPageParser
import br.ufg.inf.references.acm.ACMResultParser

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 16:44:15
 * To change this template use File | Settings | File Templates.
 */
class ACMTests extends GroovyTestCase {
    void testeXML() {
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
    }
}
