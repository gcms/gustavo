package br.gov.go.saude.hugo.utilitario

import java.text.SimpleDateFormat
import org.springframework.beans.propertyeditors.CustomDateEditor
import org.springframework.beans.PropertyEditorRegistry
import org.springframework.beans.PropertyEditorRegistrar
import org.springframework.context.MessageSource

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 29/11/2010
 * Time: 10:14:31
 * To change this template use File | Settings | File Templates.
 */
public class CustomPropertyEditorRegistrar implements PropertyEditorRegistrar {

    public void registerCustomEditors(PropertyEditorRegistry propertyEditorRegistry) {
        propertyEditorRegistry.registerCustomEditor(Date, new CustomDateEditor(UtilitarioDataHorario.dateFormat, false));
        propertyEditorRegistry.registerCustomEditor(Date, new CustomDateEditor(UtilitarioDataHorario.timeFormat, false));
    }
}
