package br.ufg.inf.references

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

        List results = client.executeQuery('allintitle:"model based"')
        results.sort { -it.links.qtd }
        results.each { println it.dump() }

        new File('/home/gustavo/modelbased.txt').text = results.collect { it.dump() }.join('\n\n')
    }

}
