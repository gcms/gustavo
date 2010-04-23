package br.ufg.inf.pbs

class HierarquiaTagLib {
    def cadaItem = { attr, body ->
        int nivel = 0

        List items = attr.items
        imprima(items , nivel, attr, body)
    }


    private void imprima(List items, int nivel, attr, body) {
        def n = attr.nivel
        def i = attr.item
        
        items.each { item ->
            out << body((n):nivel, (i):item)
            imprima(item.filhos, nivel + 1, attr, body)
        }
    }
}
