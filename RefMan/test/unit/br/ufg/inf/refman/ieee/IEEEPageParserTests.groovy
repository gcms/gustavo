package br.ufg.inf.refman.ieee

import br.ufg.inf.refman.PageProcessor
import br.ufg.inf.refman.SearchResult
import br.ufg.inf.refman.acm.ACMPageParser
import br.ufg.inf.refman.acm.ACMResultParser
import javax.xml.parsers.DocumentBuilder
import javax.xml.parsers.DocumentBuilderFactory
import org.w3c.dom.Document

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 16:44:15
 * To change this template use File | Settings | File Templates.
 */

class IEEEPageParserTests extends GroovyTestCase {
    void testeGeral() {
        DocumentBuilder dbi = DocumentBuilderFactory.newInstance().newDocumentBuilder()
        Document doc = dbi.parse(new FileInputStream('res/ieee.xml'))

        PageProcessor pp = new PageProcessor(new IEEEPageParser(), new IEEEResultParser())

        List results = pp.parsePage(doc)

        assertEquals 100, results.size()

        SearchResult first = results.first()
        assertEquals 'Model-driven development within a legacy system: an industry experience report', first.title
        assertTrue first.authors.toListString(), first.authors.contains('A. MacDonald') 
        assertEquals 3, first.authors.size()
        assertEquals 2005, first.publication.year
        assertEquals 'Software Engineering Conference', first.publication.name
        assertTrue first.description.startsWith('Model-driven development claims to off')
        assertEquals(-1, first.citationCount)
//
        assertTrue results.every { it.title.length() > 0 }
        assertTrue results.every { !it.authors.empty }
        assertTrue results.every { it.publication.name }

        SearchResult second = results[1]
        assertFalse first.title == second.title
    }
}