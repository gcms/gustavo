package br.ufg.inf.refman.ieee

import br.ufg.inf.refman.query.URLBuilder

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 15/04/2010
 * Time: 09:37:28
 * To change this template use File | Settings | File Templates.
 */

class IEEEURLBuilder implements URLBuilder {
    String buildURL(String query) {
        "http://ieeexplore.ieee.org/search/freesearchresult.jsp?queryText=${URLEncoder.encode(query)}&openedRefinements=*&searchField=Search%20All&pageNumber=1"
    }
}