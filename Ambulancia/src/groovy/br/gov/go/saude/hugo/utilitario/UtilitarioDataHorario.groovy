package br.gov.go.saude.hugo.utilitario

import java.text.DateFormat
import java.text.SimpleDateFormat
import org.codehaus.groovy.grails.commons.ApplicationHolder
import org.springframework.context.MessageSource

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 19/11/2010
 * Time: 09:46:40
 * To change this template use File | Settings | File Templates.
 */
class UtilitarioDataHorario {
    private static UtilitarioDataHorario defaultInstance

    public static UtilitarioDataHorario getDefault() {
        defaultInstance ?: (defaultInstance = createInstancia(Locale.default))
    }

    public static UtilitarioDataHorario createInstancia(Locale locale) {
        MessageSource ms = ApplicationHolder.application.mainContext.messageSource
        String dateFormat = ms?.getMessage('default.date.format', [] as Object[], locale) ?: 'dd/MM/yyy'
        String timeFormat = ms?.getMessage('default.time.format', [] as Object[], locale) ?: 'HH:mm'

        return new UtilitarioDataHorario(dateFormat, timeFormat)
    }

    // TODO: melhorar coesão... está apenas armazenando estes formatos, não utiliza para nada.
    final DateFormat dateFormat
    final DateFormat timeFormat
    
    public UtilitarioDataHorario(String dateFormat, String timeFormat) {
        this.dateFormat = new SimpleDateFormat(dateFormat)
        this.timeFormat = new SimpleDateFormat(timeFormat)
    }        

    public Date copieData(Date dataOriginal, Date dataDia) {
        copieCampos(dataOriginal, dataDia, [Calendar.YEAR, Calendar.MONTH, Calendar.DAY_OF_MONTH])
    }

    public Date copieHora(Date dataOriginal, Date dataHora) {
        copieCampos(dataOriginal, dataHora, [Calendar.HOUR_OF_DAY, Calendar.MINUTE, Calendar.SECOND])
    }

    public Date copieCampos(Date dataOriginal, Date dataNova, List campos) {
        Calendar novo = Calendar.instance
        novo.time = dataNova

        Calendar anterior = Calendar.instance
        anterior.time = dataOriginal

        campos.each { int campo ->
            anterior.set(campo, novo.get(campo))
        }

        anterior.time
    }

    public Date inicioDoDia(Date data) {
        Calendar novo = Calendar.instance
        novo.time = data

        novo.set(Calendar.HOUR_OF_DAY, 0)
        novo.set(Calendar.MINUTE, 0)
        novo.set(Calendar.SECOND, 0)

        novo.time
    }

    public Date fimDoDia(Date data) {
        Calendar novo = Calendar.instance
        novo.time = data

        novo.set(Calendar.HOUR_OF_DAY, 23)
        novo.set(Calendar.MINUTE, 59)
        novo.set(Calendar.SECOND, 59)

        novo.time
    }

    public Date inicioDoMes(Date data) {
        Calendar novo = Calendar.instance
        novo.time = data

        novo.set(Calendar.DAY_OF_MONTH, 1)

        novo.time
    }

    public Date inicioDoMes() {
        inicioDoMes(new Date())
    }
}
