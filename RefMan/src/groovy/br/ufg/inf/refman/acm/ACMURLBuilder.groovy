package br.ufg.inf.refman.acm

import br.ufg.inf.refman.query.URLBuilder

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 15/04/2010
 * Time: 09:37:28
 * To change this template use File | Settings | File Templates.
 */
class ACMURLBuilder implements URLBuilder {
    String buildURL(String query) {
        "http://portal.acm.org/results.cfm?CFID=85171659&CFTOKEN=97314837&adv=1&COLL=ACM&DL=ACM&Go.x=48&Go.y=12&termzone=Title&allofem=${URLEncoder.encode(query)}&anyofem=&noneofem=&peoplezone=Name&people=&peoplehow=and&keyword=&keywordhow=AND&affil=&affilhow=AND&pubin=&pubinhow=and&pubby=&pubbyhow=OR&since_year=&before_year=&pubashow=OR&sponsor=&sponsorhow=AND&confdate=&confdatehow=OR&confloc=&conflochow=OR&isbnhow=OR&isbn=&doi=&ccs=&subj="
    }
}
