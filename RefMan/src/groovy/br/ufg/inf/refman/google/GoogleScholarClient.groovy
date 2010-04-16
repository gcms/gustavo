package br.ufg.inf.refman.google

import br.ufg.inf.refman.Client

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:39:38
 * To change this template use File | Settings | File Templates.
 */
class GoogleScholarClient extends Client {    
    public GoogleScholarClient() {
        super(new GoogleScholarURLBuilder(), new GoogleScholarSiteDirector(),
                new GoogleScholarPageParser(), new GoogleScholarResultParser())
    }
}
