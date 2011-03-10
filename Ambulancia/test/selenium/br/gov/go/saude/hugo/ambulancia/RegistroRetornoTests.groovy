package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.ambulancia.test.AmbulanciaSeleniumAware
import grails.test.GrailsUnitTestCase

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 23/12/2010
 * Time: 08:58:20
 * To change this template use File | Settings | File Templates.
 */
@Mixin(AmbulanciaSeleniumAware)
class RegistroRetornoTests extends GrailsUnitTestCase {
    protected void setUp() {
        super.setUp()
        carregueBanco()
        login()
    }

    protected void tearDown() {
        inicio()
        logout()

        limpeBanco()
        super.tearDown()
    }

    void testeRegistroOk() {
        selenium.click("link=Registrar saída")
        selenium.waitForPageToLoad()
        selenium.selectWindow("null")
        selenium.select("motorista", "label=ELIAS COELHO")
        selenium.select("ambulancia", "label=976 - NKW-6031")
        selenium.click("//option[contains(.,'976')]")
        selenium.waitFor("Resposta ajax") {
            "0".equals(selenium.getValue("kmSaida"))
        }
        String kmSaida = selenium.getValue("kmSaida")
        selenium.click("create")
        selenium.waitForPageToLoad()

        logout()
        registreUsuario('admin', '12345', 'ROLE_ADMIN')
        login('admin', '12345')

        selenium.click("//img[@alt='Registrar retorno']")
        selenium.waitForPageToLoad()

        assertTrue(selenium.isTextPresent("Registro de retorno"))
        selenium.type("kmRetorno", kmSaida)
        selenium.click("_action_updateRetorno")
        selenium.waitForPageToLoad()

        assertTrue(selenium.isTextPresent("Retorno da viagem"))
        assertTrue(selenium.isTextPresent("foi registrado"))
        assertTrue(selenium.isTextPresent("Informações da viagem"))

        Viagem viagem = Viagem.list().first()
        assertEquals 'usuario', viagem.operador.usuario
        assertEquals 'admin', viagem.operadorRetorno.usuario
    }
}
