package br.ufg.inf.references.acm

import br.ufg.inf.references.Client
import br.ufg.inf.references.google.GoogleScholarClient

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 10:25:22
 * To change this template use File | Settings | File Templates.
 */

class Main {

    static void main(String[] args) {
        Client client = new ACMClient()

        List results = client.executeQuery('model driven')
        results.collect {
            """
${it.title} - ${it.authors.join(', ')}
${it.publication.name}, ${it.publication.year}
${it.description}
"""
        }.each { println it }
    }

}