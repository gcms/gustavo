package br.ufg.inf.refman.google

import br.ufg.inf.refman.query.URLBuilder
import br.ufg.inf.utils.url.QueryURI

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 15/04/2010
 * Time: 09:21:06
 * To change this template use File | Settings | File Templates.
 */
class GoogleScholarURLBuilder implements URLBuilder {
    String buildURL(String query) {
        "http://scholar.google.com/scholar?hl=en&as_subj=eng&q=${URLEncoder.encode(query)}"
    }
}
