package br.gov.go.saude.hugo.ambulancia

import grails.test.GrailsUnitTestCase
import br.gov.go.saude.hugo.ambulancia.test.AmbulanciaSeleniumAware

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 23/12/2010
 * Time: 08:58:20
 * To change this template use File | Settings | File Templates.
 */
@Mixin(AmbulanciaSeleniumAware)
class RegistroSaidaTests extends GrailsUnitTestCase {
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
        assertTrue(selenium.isTextPresent("Registro de saída"))

        assertFalse(selenium.isElementPresent("//select[@id='ambulancia']/option[contains(., '1010 - NKV-4401')]"))
        assertTrue(selenium.isElementPresent("//select[@id='ambulancia']/option[contains(., '976 - NKW-6031')]"))

        assertTrue(selenium.isTextPresent("Registro de saída"))
        selenium.selectWindow("null")
        selenium.select("motorista", "label=ELIAS COELHO")
        selenium.select("ambulancia", "label=976 - NKW-6031")
        selenium.click("//option[contains(.,'976')]")

        selenium.waitFor("Resposta ajax") {
            "0".equals(selenium.getValue("kmSaida"))
        }

        selenium.click("create")
        selenium.waitForPageToLoad()
        assertTrue(selenium.isTextPresent("Viagem registrada com o identificador"))
    }

    void testeRegistroNenhumCampoPreenchido() {
        selenium.click("link=Registrar saída")
        selenium.waitForPageToLoad()

        selenium.click("create")
        selenium.waitForPageToLoad()

        assertTrue(selenium.isTextPresent("O campo [ambulancia] é obrigatório"))
        assertTrue(selenium.isTextPresent("O campo [kmSaida] é obrigatório"))
        assertTrue(selenium.isTextPresent("O campo [motorista] é obrigatório"))
    }

    void testeRegistroSemAmbulancia() {
        selenium.click("link=Registrar saída")
        selenium.waitForPageToLoad()

        selenium.selectWindow("null")
        selenium.select("motorista", "label=ELIAS COELHO")

        selenium.click("create")
        selenium.waitForPageToLoad()

        assertTrue(selenium.isTextPresent("O campo [ambulancia] é obrigatório"))
        assertTrue(selenium.isTextPresent("O campo [kmSaida] é obrigatório"))
    }

    void testeRegistroSemMotorista() {
        selenium.click("link=Registrar saída")
        selenium.waitForPageToLoad()

        selenium.selectWindow("null")
        selenium.select("ambulancia", "label=976 - NKW-6031")
        selenium.click("//option[contains(.,'976')]")

        selenium.waitFor("Resposta ajax") {
            "0".equals(selenium.getValue("kmSaida"))
        }

        selenium.click("create")
        selenium.waitForPageToLoad()

        assertTrue(selenium.isTextPresent("O campo [motorista] é obrigatório"))
    }

    void testeRegistroOkComPaciente() {
        selenium.click("link=Registrar saída")
        selenium.waitForPageToLoad()
        assertTrue(selenium.isTextPresent("Registro de saída"))

        assertFalse(selenium.isElementPresent("//select[@id='ambulancia']/option[contains(., '1010 - NKV-4401')]"))
        assertTrue(selenium.isElementPresent("//select[@id='ambulancia']/option[contains(., '976 - NKW-6031')]"))

        assertTrue(selenium.isTextPresent("Registro de saída"))
        selenium.selectWindow("null")
        selenium.select("motorista", "label=ELIAS COELHO")
        selenium.select("ambulancia", "label=976 - NKW-6031")
        selenium.click("//option[contains(.,'976')]")

        selenium.waitFor("Resposta ajax") {
            "0".equals(selenium.getValue("kmSaida"))
        }

        selenium.click("link=paciente")
        selenium.type("ext-comp-1004", "Fulano da Silva")
        selenium.type("ext-comp-1005", "123")
        selenium.type("ext-comp-1006", "Hospital XYZ")

        selenium.click("create")
        selenium.waitForPageToLoad()
        assertTrue(selenium.isTextPresent("Viagem registrada com o identificador"))

        assertTrue(selenium.isElementPresent("//tr/td[contains(., '976 - NKW-6031')]"))
        assertEquals("HOSPITAL XYZ", selenium.getText("//tr[contains(., '976 - NKW-6031')]/td[5]"))
        assertEquals("Fulano da Silva", selenium.getText("//tr[contains(., '976 - NKW-6031')]/td[6]"))
        assertEquals("0", selenium.getText("//tr[contains(., '976 - NKW-6031')]/td[9]"))

        assertEquals 1, Viagem.list().size()
        Viagem viagem = Viagem.list().first()
        assertEquals 1, viagem.paradas.size()
    }

    void testeAlteracaoKmSaida() {
        testeRegistroOkComPaciente()

        selenium.click("//tr[contains(., '976 - NKW-6031')]/td[1]/a")
        selenium.waitForPageToLoad()
        assertTrue(selenium.isTextPresent("Informações da viagem"))

        selenium.click('_action_editSaida')
        selenium.waitForPageToLoad()
        assertTrue(selenium.isTextPresent("Alteração de viagem"))

        selenium.type("name=kmSaida", "10")
        selenium.click('_action_updateSaida')
        selenium.waitForPageToLoad()

        assertTrue(selenium.isTextPresent("Viagem atualizada com o identificador"))
        assertEquals("10", selenium.getText("//tr[contains(., '976 - NKW-6031')]/td[9]"))
    }
}
