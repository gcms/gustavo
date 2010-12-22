package br.gov.go.saude.hugo.utilitario

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 07/12/2010
 * Time: 12:11:19
 * To change this template use File | Settings | File Templates.
 */
class UtilitarioNome {
    public static String formateNomePessoa(String nome) {
        if (nome == null)
            return nome

        List nomes = nome.split('\\s+') as List
        nomes.size() == 1 ? nome.toUpperCase() : "${nomes.first()} ${nomes.last()}".toUpperCase()
    }
}
