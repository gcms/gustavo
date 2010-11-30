package br.gov.go.saude.hugo.utilitario

import org.codehaus.groovy.grails.web.binding.GrailsDataBinder

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 29/11/2010
 * Time: 12:09:55
 * To change this template use File | Settings | File Templates.
 */
class CustomGrailsDataBinder extends GrailsDataBinder {

    public CustomGrailsDataBinder(target, objectName) {
        super(target, objectName)

        println "new CustomGrailsDataBinder"
    }
}
