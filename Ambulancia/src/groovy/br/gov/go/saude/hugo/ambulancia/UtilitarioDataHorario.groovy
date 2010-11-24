package br.gov.go.saude.hugo.ambulancia

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 19/11/2010
 * Time: 09:46:40
 * To change this template use File | Settings | File Templates.
 */
class UtilitarioDataHorario {
    public static Date copieData(Date dataOriginal, Date dataDia) {
        copieCampos(dataOriginal, dataDia, [Calendar.YEAR, Calendar.MONTH, Calendar.DAY_OF_MONTH])
    }

    public static Date copieHora(Date dataOriginal, Date dataHora) {
        copieCampos(dataOriginal, dataHora, [Calendar.HOUR_OF_DAY, Calendar.MINUTE, Calendar.SECOND])
    }

    public static Date copieCampos(Date dataOriginal, Date dataNova, List campos) {
        Calendar novo = Calendar.instance
        novo.time = dataNova

        Calendar anterior = Calendar.instance
        anterior.time = dataOriginal

        campos.each { int campo ->
            anterior.set(campo, novo.get(campo))
        }

        anterior.time
    }

    public static Date inicioDoDia(Date data) {
        Calendar novo = Calendar.instance
        novo.time = data

        novo.set(Calendar.HOUR_OF_DAY, 0)
        novo.set(Calendar.MINUTE, 0)
        novo.set(Calendar.SECOND, 0)

        novo.time
    }

    public static Date fimDoDia(Date data) {
        Calendar novo = Calendar.instance
        novo.time = data

        novo.set(Calendar.HOUR_OF_DAY, 23)
        novo.set(Calendar.MINUTE, 59)
        novo.set(Calendar.SECOND, 59)

        novo.time
    }
    
    public static Date inicioDoMes(Date data) {
        Calendar novo = Calendar.instance
        novo.time = data

        novo.set(Calendar.DAY_OF_MONTH, 1)

        novo.time
    }

    public static Date inicioDoMes() {
        inicioDoMes(new Date())
    }
}
