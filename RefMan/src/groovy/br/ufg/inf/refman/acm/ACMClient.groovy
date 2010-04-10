package br.ufg.inf.refman.acm

import br.ufg.inf.refman.Client

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:52:33
 * To change this template use File | Settings | File Templates.
 */
class ACMClient extends Client {
    public ACMClient() {
        super(new ACMSiteDirector(), new ACMPageParser(), new ACMResultParser())
    }
}
