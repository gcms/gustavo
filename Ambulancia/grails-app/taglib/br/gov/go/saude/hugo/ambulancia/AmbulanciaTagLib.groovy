package br.gov.go.saude.hugo.ambulancia

import grails.plugins.springsecurity.SecurityTagLib
import org.codehaus.groovy.grails.plugins.web.taglib.ApplicationTagLib
import org.springframework.beans.factory.InitializingBean
import org.springframework.context.ApplicationContext
import org.springframework.context.ApplicationContextAware
import org.codehaus.groovy.grails.commons.GrailsApplication
import org.codehaus.groovy.grails.plugins.web.taglib.FormTagLib

class AmbulanciaTagLib implements ApplicationContextAware, InitializingBean {
    ApplicationContext applicationContext

    ApplicationTagLib applicationTagLib
    FormTagLib formTagLib
    SecurityTagLib securityTagLib

    GrailsApplication application

    private String getURL(attrs) {
        applicationTagLib.createLink(attrs.clone()).
                replace("/${application.metadata['app.name']}", '')
    }
    
    def linkIfAccess = { attrs, body ->
        def url = getURL(attrs)
        def content = applicationTagLib.link(attrs, body)
        out << securityTagLib.access(url: url, content)
    }

    def actionSubmitIfAccess = { attrs, body ->
        def url = getURL(attrs)
        def content = formTagLib.actionSubmit(attrs, body)
        out << securityTagLib.access(url: url, content)
    }

    void afterPropertiesSet() {
        applicationTagLib = applicationContext.getBean(ApplicationTagLib)
        securityTagLib = applicationContext.getBean(SecurityTagLib)
        formTagLib = applicationContext.getBean(FormTagLib)
        application = applicationContext.getBean(GrailsApplication)
    }
}
