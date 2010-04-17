package br.ufg.inf.refman.ieee

import br.ufg.inf.refman.Client
import br.ufg.inf.refman.acm.ACMPageParser
import br.ufg.inf.refman.acm.ACMResultParser
import br.ufg.inf.refman.acm.ACMSiteDirector
import br.ufg.inf.refman.acm.ACMURLBuilder

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 09/04/2010
 * Time: 11:52:33
 * To change this template use File | Settings | File Templates.
 */

class IEEEClient extends Client {
    public IEEEClient() {
        super(new IEEEURLBuilder(), new IEEESiteDirector(), new IEEEPageParser(), new IEEEResultParser())
    }
}