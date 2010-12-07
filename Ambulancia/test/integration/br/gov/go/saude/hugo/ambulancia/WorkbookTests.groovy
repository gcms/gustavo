package br.gov.go.saude.hugo.ambulancia

import br.gov.go.saude.hugo.ambulancia.importacao.ImportadorPlanilha
import br.gov.go.saude.hugo.ambulancia.importacao.ImportadorPlanilhaBanco

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 03/12/2010
 * Time: 12:19:43
 * To change this template use File | Settings | File Templates.
 */
class WorkbookTests extends GroovyTestCase {
    ImportadorPlanilha importador = new ImportadorPlanilha()


    void testeLeituraXLS() {
        assertTrue Viagem.list().size() == 0

        new ImportadorPlanilhaBanco(caminho: new File('res')).importeArquivos(['DEZEMBRO'])

        assertTrue Viagem.list().size() > 20
    }
}
