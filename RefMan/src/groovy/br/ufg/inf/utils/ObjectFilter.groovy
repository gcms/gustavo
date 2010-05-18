package br.ufg.inf.utils

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 14/05/2010
 * Time: 13:56:01
 * To change this template use File | Settings | File Templates.
 */
class ObjectFilter {
    public static List filter(List itens, Map params) {
        params.offset = params.offset ? params.int('offset') : params.offset
        params.max = params.max ? params.int('max') : params.max

        if (params.sort)
            itens = itens.sort { it."${params.sort}" }
        if (params.order == 'desc')
            itens = itens.reverse()
        if (params.offset)
            itens = itens[params.offset..<itens.size()]        
        if (params.max)
            itens = itens.size() > params.max ? itens[0..<params.max] : itens

        itens            
    }
}
