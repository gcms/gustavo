package br.ufg.inf.utils

/**
 * Created by IntelliJ IDEA.
 * User: Noturno
 * Date: 17/04/2010
 * Time: 08:50:50
 * To change this template use File | Settings | File Templates.
 */
class Normalizer {
    static String normalize(String title) {
        List parts = title.toLowerCase().split('\\W+')
        parts.join(' ')
    }
}
