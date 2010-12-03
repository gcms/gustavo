package br.gov.go.saude.hugo.ambulancia

import org.codehaus.groovy.grails.commons.GrailsApplication
import org.springframework.beans.factory.InitializingBean
import org.springframework.context.ApplicationContext
import org.springframework.context.ApplicationContextAware

class AmbulanciaTagLib implements ApplicationContextAware, InitializingBean {
    ApplicationContext applicationContext
    GrailsApplication application

    private String getURL(attrs) {
        g.createLink(attrs.clone()).
                replace("/${application.metadata['app.name']}", '')
    }
    
    def linkIfAccess = { attrs, body ->
        def url = getURL(attrs)
        def content = g.link(attrs, body)
        out << sec.access(url: url, content)
    }

    def actionSubmitIfAccess = { attrs, body ->
        def url = getURL(attrs)
        def content = g.actionSubmit(attrs, body)
        out << sec.access(url: url, content)
    }

    void afterPropertiesSet() {
        application = applicationContext.getBean(GrailsApplication)
    }
}
