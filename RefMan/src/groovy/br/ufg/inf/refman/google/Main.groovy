package br.ufg.inf.refman.google

import br.ufg.inf.refman.Client

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 10:25:22
 * To change this template use File | Settings | File Templates.
 */
class Main {

    static void main(String[] args) {
        Client client = new GoogleScholarClient()

        List results = client.executeURL('allintitle:"model based software engineering"')
        results.each { println it.dump() }

        new File('/home/gustavo/MDE.txt').text = results.collect { it.dump() }.join('\n\n')
    }

}
