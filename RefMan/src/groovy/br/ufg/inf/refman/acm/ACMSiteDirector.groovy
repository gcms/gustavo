package br.ufg.inf.refman.acm

import br.ufg.inf.refman.SiteDirector
import br.ufg.inf.utils.url.QueryURI
import org.apache.log4j.Logger

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:50:45
 * To change this template use File | Settings | File Templates.
 */
class ACMSiteDirector implements SiteDirector {
    private static Logger log = Logger.getLogger(ACMSiteDirector)

    public List collectPages(String url, Closure closure) {
        List result = []
        int start = 0

        log.debug "URL: ${url}"
        new ACMRandomFetcher(url, closure).fetchAll()
    }
}

class ACMRandomFetcher {
    int max = 10
    Map pages = [:]
    ACMPageFetcher fetcher

    public ACMRandomFetcher(String url, Closure closure) {
        fetcher = new ACMPageFetcher(url, closure)
    }

    private int nextPageNumber() {
        assert pages.size() < max

        while (true) {
            int nextPage = Math.random() * max

            if (!pages[nextPage])
                return nextPage
        }
    }

    public List fetchNextPage(int nextPage) {
        assert nextPage >= 0 && nextPage < max
        assert !pages[nextPage]

        List itens = fetcher.getPage(nextPage)
        if (itens.size() == 0) {
            max = nextPage
            return null
        } else if (itens.size() < 20) {
            max = nextPage + 1
        } else if (nextPage == max - 1) { // && itens.size() == 20
            max += Math.random() * 10
        }

        return itens
    }

    public List fetchAll() {
        while (pages.size() < max) {
            int nextPage = nextPageNumber()
            List page = fetchNextPage(nextPage)
            if (page)
                pages[nextPage] = page
        }

        pages.values().flatten()
    }
}

class ACMPageFetcher {
    QueryURI queryURI
    Closure transform

    public ACMPageFetcher(String url, Closure closure) {
        queryURI = new QueryURI(url)
        transform = closure
    }

    public List getPage(int pageNumber) {
        queryURI.setQueryParam('start', pageNumber * 20 + 1)
        transform(queryURI.getURI())
    }
}