package br.ufg.inf.scholar

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 10:25:22
 * To change this template use File | Settings | File Templates.
 */
class Main {

    static void main(String[] args) {
        Client client = new Client()

        List results = client.executeQuery('allintitle:model-driven')
        results.sort { -it.links.qtd }
        results.each { println it.dump() }

        new File('/home/gustavo/model-driven.txt').text = results.join('\n\n')
    }

}
