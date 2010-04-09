package br.ufg.inf.references.google

import br.ufg.inf.references.SiteDirector

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 09:47:27
 * To change this template use File | Settings | File Templates.
 */
class GoogleScholarSiteDirector implements SiteDirector {

    public List collectPages(String query, Closure closure) {
        List result = []
        int num = 100
        int start = 0

        while (true) {
            String url = "http://scholar.google.com/scholar?hl=en&as_subj=eng&q=${URLEncoder.encode(query)}&num=${num}&start=${start}"

            List itens = closure(new URI(url))
            result.addAll(itens)
            if (itens.size() < num)
                break

            start += num
        }

        result
    }    
}
