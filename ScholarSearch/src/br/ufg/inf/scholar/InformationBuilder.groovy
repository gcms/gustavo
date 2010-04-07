package br.ufg.inf.scholar

import java.util.regex.Matcher

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 11:29:28
 * To change this template use File | Settings | File Templates.
 */
class InformationBuilder {
    List authors
    Publication publication

    public InformationBuilder() {}

    public void parseAuthorLine(String line) {
        String[] parts = line.split('-', 2)

        parseAuthors(parts[0].trim())
        if (parts.length > 1)
            parsePublication(parts[1].trim())
    }

    private void parseAuthors(String authorsPart) {
        authors = authorsPart.tokenize(',').collect { it.trim() }
    }

    private void parsePublication(String publicationPart) {
        String name = getName(publicationPart)
        String year = getYear(publicationPart)

        publication = new Publication(name: name, year: year?.trim()?.toInteger())
    }

    public String getYear(String text) {
        List itens = getItems(text)
        String year = itens.find { isYear(it) }

        if (year)
            return year

        for (String item: itens) {
            Matcher m = (item =~ ~/\d{4}/)
            while (m.find()) {
                year = m.group()
                if (isYear(year))
                    return year
            }
        }

        return null
    }

    private boolean isYear(String texto) {
        texto.isNumber() && texto.toInteger() > 1800 && texto.toInteger() < 2020 // FIXME
    }

    private List getItems(String publicationPart) {
        (publicationPart.split("[,-]") as List).collect { it.trim() }
    }


    public String getName(String text) {
        List itens = getItems(text)

        itens = itens.findAll { !it.trim().isNumber() }
        if (itens.size() > 1)
            itens = itens.findAll { !(it =~ ~/.*[a-z]+\.[a-z]+\.*/)}
        itens = itens.sort { -it.size() }

        !itens.empty ? itens.first() : null
    }
}

