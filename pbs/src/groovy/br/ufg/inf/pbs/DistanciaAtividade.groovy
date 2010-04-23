package br.ufg.inf.pbs

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 23/04/2010
 * Time: 15:44:35
 * To change this template use File | Settings | File Templates.
 */
class DistanciaAtividade {
    public static int calculeDistanciaAtividades(Atividade cursor, Atividade outra) {
        int distancia = 0

        while (cursor) {
            if (cursor == outra)
                return distancia
            distancia++
            cursor = cursor.pai
        }

        (distancia + 1) + outra.id
    }
}
