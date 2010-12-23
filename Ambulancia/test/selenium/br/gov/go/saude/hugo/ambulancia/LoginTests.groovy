package br.gov.go.saude.hugo.ambulancia

import grails.test.GrailsUnitTestCase
import grails.plugins.selenium.SeleniumAware
import br.gov.go.saude.hugo.ambulancia.utilitario.Ambiente
import br.gov.go.saude.hugo.ambulancia.test.AmbulanciaSeleniumAware

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 23/12/2010
 * Time: 08:13:20
 * To change this template use File | Settings | File Templates.
 */
@Mixin(AmbulanciaSeleniumAware)
class LoginTests extends GrailsUnitTestCase {
    void testeLogin() {
        selenium.open "/${Ambiente.instancia.appName}/"

        login()

        assertEquals("Viagens", selenium.getTitle())
        selenium.isTextPresent("Admin")
        selenium.isTextPresent("Sessão iniciada em")
        selenium.isTextPresent("Viagens em trânsito")

        logout()
    }
}
