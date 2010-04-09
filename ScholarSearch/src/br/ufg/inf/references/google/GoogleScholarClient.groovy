package br.ufg.inf.references.google

import br.ufg.inf.references.Client

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:39:38
 * To change this template use File | Settings | File Templates.
 */
class GoogleScholarClient extends Client {    
    public GoogleScholarClient() {
        super(new GoogleScholarSiteDirector(), new GoogleScholarPageParser(), new GoogleScholarResultParser())
    }
}
