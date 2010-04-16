package br.ufg.inf.refman.google

import br.ufg.inf.refman.SiteDirector
import br.ufg.inf.utils.url.QueryURI

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 09:47:27
 * To change this template use File | Settings | File Templates.
 */
class GoogleScholarSiteDirector implements SiteDirector {

    public List collectPages(String url, Closure closure) {
        List result = []
        int num = 100
        int start = 0

        QueryURI queryURI = new QueryURI(url)
        while (true) {
            queryURI.setQueryParam('start', start)
            queryURI.setQueryParam('num', num)

            List itens = closure(queryURI.getURI())
            result.addAll(itens)
            if (itens.size() < num)
                break

            start += num
        }

        result
    }    
}
