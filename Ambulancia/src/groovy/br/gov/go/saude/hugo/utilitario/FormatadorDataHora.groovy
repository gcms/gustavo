package br.gov.go.saude.hugo.utilitario

import org.springframework.context.MessageSource
import org.codehaus.groovy.grails.commons.ApplicationHolder
import java.text.DateFormat
import java.text.SimpleDateFormat

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 08/02/2011
 * Time: 10:49:01
 * To change this template use File | Settings | File Templates.
 */
class FormatadorDataHora {
    private static FormatadorDataHora defaultInstance

    public static FormatadorDataHora getDefault() {
            defaultInstance ?: (defaultInstance = createInstancia(Locale.default))
    }

    public static FormatadorDataHora createInstancia(Locale locale) {
        MessageSource ms = ApplicationHolder.application.mainContext.messageSource
        String dateFormat = ms?.getMessage('default.date.format', [] as Object[], locale) ?: 'dd/MM/yyy'
        String timeFormat = ms?.getMessage('default.time.format', [] as Object[], locale) ?: 'HH:mm'

        return new FormatadorDataHora(dateFormat, timeFormat)
    }

    final DateFormat dateFormat
    final DateFormat timeFormat

    public FormatadorDataHora(String dateFormat, String timeFormat) {
        this.dateFormat = new SimpleDateFormat(dateFormat)
        this.timeFormat = new SimpleDateFormat(timeFormat)
    }

}
