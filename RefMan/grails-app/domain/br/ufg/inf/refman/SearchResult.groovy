package br.ufg.inf.refman

import br.ufg.inf.utils.LevenshteinDistance

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 11:02:45
 * To change this template use File | Settings | File Templates.
 */
class SearchResult {
    static constraints = {
        queryResult(nullable: false)
        title(nullable: false, blank: false)
        url(nullable: true)
        authorsString(nullable: false, blank: true)
        publicationName(nullable: true)
        publicationYear(nullable: true)
        html(nullable: false)
    }

    static belongsTo = QueryResult
    QueryResult queryResult

    String title
    String url

    String authorsString = ''
    String publicationName
    Integer publicationYear

    static transients = [ 'normalizedTitle', 'authors', 'publication' ]

    public Publication getPublication() {
        new Publication(year: publicationYear, name: publicationName)
    }

    public void setPublication(Publication publication) {
        if (publication != null) {
            publicationYear = publication.year
            publicationName = publication.name
        }
    }

    int citationCount

    String description
    List links = []
    String html

    public void setAuthors(List authors) {
        authorsString = authors.join(', ')
    }

    public List getAuthors() {
        authorsString.tokenize(',').collect { it.trim() }
    }

    public double calculateDistanceToLengthRation(SearchResult searchResult) {
        int distance = LevenshteinDistance.calcule(normalizedTitle, searchResult.normalizedTitle)

        distance / searchResult.normalizedTitle.length() + distance / normalizedTitle.length()
    }

    public String getNormalizedTitle() {
        title.toLowerCase().tokenize('\\W+').join(' ')
    }
    //String type
}
