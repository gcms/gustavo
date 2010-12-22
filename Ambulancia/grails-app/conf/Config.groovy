import org.apache.log4j.DailyRollingFileAppender
import br.gov.go.saude.hugo.utilitario.Ambiente

// locations to search for config files that get merged into the main config
// config files can either be Java properties files or ConfigSlurper scripts

// grails.config.locations = [ "classpath:${appName}-config.properties",
//                             "classpath:${appName}-config.groovy",
//                             "file:${userHome}/.grails/${appName}-config.properties",
//                             "file:${userHome}/.grails/${appName}-config.groovy"]

// if(System.properties["${appName}.config.location"]) {
//    grails.config.locations << "file:" + System.properties["${appName}.config.location"]
// }

grails.project.groupId = appName // change this to alter the default package name and Maven publishing destination
grails.mime.file.extensions = true // enables the parsing of file extensions from URLs into the request format
grails.mime.use.accept.header = false
grails.mime.types = [html: ['text/html', 'application/xhtml+xml'],
        xml: ['text/xml', 'application/xml'],
        text: 'text/plain',
        js: 'text/javascript',
        rss: 'application/rss+xml',
        atom: 'application/atom+xml',
        css: 'text/css',
        csv: 'text/csv',
        all: '*/*',
        json: ['application/json', 'text/json'],
        form: 'application/x-www-form-urlencoded',
        multipartForm: 'multipart/form-data'
]

// URL Mapping Cache Max Size, defaults to 5000
//grails.urlmapping.cache.maxsize = 1000

// The default codec used to encode data with ${}
grails.views.default.codec = "none" // none, html, base64
grails.views.gsp.encoding = "UTF-8"
grails.converters.encoding = "UTF-8"
// enable Sitemesh preprocessing of GSP pages
grails.views.gsp.sitemesh.preprocess = true
// scaffolding templates configuration
grails.scaffolding.templates.domainSuffix = 'Instance'

// Set to false to use the new Grails 1.2 JSONBuilder in the render method
grails.json.legacy.builder = false
// enabled native2ascii conversion of i18n properties files
grails.enable.native2ascii = true
// whether to install the java.util.logging bridge for sl4j. Disable for AppEngine!
grails.logging.jul.usebridge = true
// packages to include in Spring bean scanning
grails.spring.bean.packages = []

// set per-environment serverURL stem for creating absolute links
environments {
    production {
        grails.serverURL = "http://www.changeme.com"
    }
    development {
        grails.serverURL = "http://localhost:8080/${appName}"
    }
    test {
        grails.serverURL = "http://localhost:8080/${appName}"
    }

}

// log4j configuration
log4j = {
    // Example of changing the log pattern for the default console
    // appender:
    //
    appenders {
//        file name:'file', file: "${grails.util.Environment.currentEnvironment}.log"
        appender new DailyRollingFileAppender(name: 'stdout', datePattern: "'.'yyyy-MM-dd", file: Ambiente.instancia.logFile, layout: pattern(conversionPattern: '[%d{yyyy-MM-dd hh:mm:ss.SSS}] %p %c - %m%n'))

//        rollingFile name:'stdout', maxFileSize:1024*1024/2, file:"${grails.util.Environment.currentEnvironment}.log"
//        console name:'stdout', layout:pattern(conversionPattern: '[%d] %p %t %c - %m%n')
    }

    error  'org.codehaus.groovy.grails.web.servlet',  //  controllers
           'org.codehaus.groovy.grails.web.pages', //  GSP
           'org.codehaus.groovy.grails.web.sitemesh', //  layouts
           'org.codehaus.groovy.grails.web.mapping.filter', // URL mapping
           'org.codehaus.groovy.grails.web.mapping', // URL mapping
           'org.codehaus.groovy.grails.commons', // core / classloading
           'org.codehaus.groovy.grails.plugins', // plugins
           'org.codehaus.groovy.grails.orm.hibernate', // hibernate integration
           'org.springframework',
           'org.hibernate',
           'net.sf.ehcache.hibernate'

    warn  'org.mortbay.log'

    debug 'grails.app'

    debug 'br.gov.go.saude.hugo'
}

// Added by the Spring Security Core plugin:
grails.plugins.springsecurity.userLookup.userDomainClassName = 'br.gov.go.saude.hugo.ambulancia.Operador'
grails.plugins.springsecurity.userLookup.usernamePropertyName = 'usuario'
grails.plugins.springsecurity.userLookup.passwordPropertyName = 'senha'

//grails.plugins.springsecurity.userLookup.authoritiesPropertyName = 'grupos'
grails.plugins.springsecurity.userLookup.authorityJoinClassName = 'br.gov.go.saude.hugo.ambulancia.OperadorGrupo'

grails.plugins.springsecurity.authority.className = 'br.gov.go.saude.hugo.ambulancia.Grupo'
//grails.plugins.springsecurity.authority.nameField = 'authority'


grails.plugins.springsecurity.rejectIfNoRule = true
grails.plugins.springsecurity.securityConfigType = grails.plugins.springsecurity.SecurityConfigType.InterceptUrlMap

grails.plugins.springsecurity.interceptUrlMap = [
        '/**': ['IS_AUTHENTICATED_REMEMBERED'],
        '/js/**': ['IS_AUTHENTICATED_ANONYMOUSLY'],
        '/css/**': ['IS_AUTHENTICATED_ANONYMOUSLY'],
        '/images/**': ['IS_AUTHENTICATED_ANONYMOUSLY'],
        '/login/**': ['IS_AUTHENTICATED_ANONYMOUSLY'],
        '/logout/**': ['IS_AUTHENTICATED_ANONYMOUSLY'],

        '/operador/**': ['ROLE_SUPERUSER'],
//   '/operador/index':    ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/operador/list': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/operador/show/*': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],

        '/ambulancia/**': ['ROLE_ADMIN', 'ROLE_SUPERUSER'],
//   '/ambulancia/index':  ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/ambulancia/list': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/ambulancia/show/*': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],

        '/motorista/**': ['ROLE_ADMIN', 'ROLE_SUPERUSER'],
//   '/motorista/index':  ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/motorista/list': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/motorista/show/*': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],

        '/viagem/**': ['ROLE_SUPERUSER'],
        '/viagem/createSaida': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/saveSaida':   ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/deleteSaida':   ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/showSaida/*':   ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/list':   ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/print':   ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],

        '/viagem/editRetorno/*': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/updateRetorno': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],

        '/viagem/show/*': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],

        '/viagem/listDestinos': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        '/viagem/obtenhaKmAmbulancia': ['ROLE_USER', 'ROLE_ADMIN', 'ROLE_SUPERUSER'],
        
        '/console/**': ['ROLE_SUPERUSER']
]