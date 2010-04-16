package br.ufg.inf.refman.query

import br.ufg.inf.utils.url.QueryURI

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 14/04/2010
 * Time: 13:27:49
 * To change this template use File | Settings | File Templates.
 */
interface URLBuilder {
    String buildURL(String query)
}
