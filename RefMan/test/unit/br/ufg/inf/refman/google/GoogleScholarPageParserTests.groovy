package br.ufg.inf.refman.google

import br.ufg.inf.refman.PageProcessor
import br.ufg.inf.refman.SearchResult
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

class GoogleScholarPageParserTests extends GroovyTestCase {
    void testeGeral() {
        DocumentBuilder dbi = DocumentBuilderFactory.newInstance().newDocumentBuilder()
        Document doc = dbi.parse(new FileInputStream('res/scholar.xml'))

        PageProcessor pp = new PageProcessor(new GoogleScholarPageParser(), new GoogleScholarResultParser())

        List results = pp.parsePage(doc)        

        SearchResult first = results.first()
        assertEquals 'Model-based Software Engineering', first.title
        assertEquals 2, first.citationCount
        assertTrue first.authors.contains('K Spies')

        assertEquals 43, results.max { it.citationCount }.citationCount        
        assertEquals 32, results.size()
    }
}