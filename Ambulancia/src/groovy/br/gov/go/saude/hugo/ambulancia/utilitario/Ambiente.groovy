package br.gov.go.saude.hugo.ambulancia.utilitario

import org.codehaus.groovy.grails.commons.ApplicationHolder
import grails.util.Environment

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 22/12/2010
 * Time: 10:08:07
 * To change this template use File | Settings | File Templates.
 */
class Ambiente {
    private static Ambiente instancia
    public static Ambiente getInstancia() {
        instancia ?: (instancia = new Ambiente())
    }

    private Ambiente() {        
    }

    public File getLogFolder(String appName) {
        production ? new File(catalinaHome, "logs/$appName") : new File(".")
    }

    boolean isProduction() {
        Environment.current == Environment.PRODUCTION
    }

    String getCatalinaHome() {
        System.getProperty("catalina.base") ?: "."
    }

    public File getLogFile(String appName) {
        new File(getLogFolder(appName), "${Environment.current}.log")
    }

    public String getAppName() {
        ApplicationHolder.application.metadata['app.name']
    }
}
