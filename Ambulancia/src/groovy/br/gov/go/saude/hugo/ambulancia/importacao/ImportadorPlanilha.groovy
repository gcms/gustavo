package br.gov.go.saude.hugo.ambulancia.importacao

import org.apache.poi.ss.usermodel.Sheet
import org.apache.poi.ss.usermodel.Row
import org.apache.poi.ss.usermodel.Cell
import java.text.SimpleDateFormat
import org.apache.poi.hssf.usermodel.HSSFWorkbook
import org.apache.poi.ss.usermodel.Workbook
import br.gov.go.saude.hugo.utilitario.GerenciadorStringsSemelhantes
import br.gov.go.saude.hugo.utilitario.ComparadorNomesSemelhantes
import br.gov.go.saude.hugo.ambulancia.Ambulancia
import br.gov.go.saude.hugo.ambulancia.Motorista
import br.gov.go.saude.hugo.ambulancia.ParadaPaciente
import br.gov.go.saude.hugo.ambulancia.Operador

/**
 * Created by IntelliJ IDEA.
 * User: gustavosousa
 * Date: 06/12/2010
 * Time: 10:05:08
 * To change this template use File | Settings | File Templates.
 */
class ImportadorPlanilha {
    GerenciadorAmbulancias ambulancias = new GerenciadorAmbulancias()
    GerenciadorMotoristas motoristas = new GerenciadorMotoristas()
    GerenciadorOperadores operadores = new GerenciadorOperadores()
    GerenciadorStringsSemelhantes semelhantes = new GerenciadorStringsSemelhantes(ComparadorNomesSemelhantes.instancia)

    List parseSheet(Sheet sheet) {
        List itens = []
        Iterator itr = sheet.rowIterator()

        while (itr.hasNext()) {
            Row linha = itr.next()

            Object item = parseItem(linha, itr)
            if (item) {
                itens.add(item)
            }
        }

        itens
    }

    String parseStringCellRange(Row row, Range range) {
        range.collect {
            parseString(row.getCell(it)).trim()
        }.join(' ').trim()
    }

    String parseIntString(Cell cell) {
        if (cell == null)
            return null

        if (cell.cellType == Cell.CELL_TYPE_BLANK)
            return ''

        cell.cellType == Cell.CELL_TYPE_STRING ? cell.stringCellValue : '' + (int) cell.numericCellValue
    }

    String parseString(Cell cell) {
        if (cell == null)
            return null

        if (cell.cellType == Cell.CELL_TYPE_BLANK)
            return ''

        cell.cellType == Cell.CELL_TYPE_STRING ? cell.stringCellValue : '' + cell.numericCellValue
    }

    String parsePlaca(String placa) {
        //println "Placa original: $placa"
        placa.replaceAll('\\W', '')
    }

    Ambulancia crieAmbulancia(String prefixo, String placa) {
        assertEquals 7, placa.replaceAll('\\W', '').length()

        new Ambulancia(prefixo: prefixo, placa: placa)
    }

    Ambulancia parseAmbulancia(String prefixo, String placa) {
//        println "Parsing ambulancia... $prefixo/$placa"

        Ambulancia nova = new Ambulancia(prefixo: prefixo, placa: placa)

        Ambulancia obtida = ambulancias.obtenha(prefixo, placa)

        //println "obtida: $obtida"
        if (obtida == null) {
            assertEquals 7, placa.length()
            nova.placa = placa[0..2] + '-' + placa[3..6]
        }
        //println "Nova: $nova"

        ambulancias.adicione(obtida, nova)
    }

    Motorista parseMotorista(String nome) {
        //println "Parsing motorista.. $nome"
        Motorista novo = new Motorista(nome: nome)
        Motorista obtido = motoristas.obtenha(nome)

        motoristas.adicione(obtido, novo)
    }

    Date parseHora(Cell cell) {
        //println "Parsing hora..."

        if (cell.cellType == Cell.CELL_TYPE_BLANK)
            return null

        if (cell.cellType == Cell.CELL_TYPE_NUMERIC)
            return cell.dateCellValue

        String hora = cell.stringCellValue
        //println "Hora: $hora"

        if (hora.trim().empty)
            return null

        hora = hora.replace(';', ':').trim()

        if (hora =~ ~/\d{2}:\d{2}/)
            return new SimpleDateFormat("HH:mm").parse(hora)

        if (hora =~ ~/d{2}:\d{2}:\d{2}/)
            return new SimpleDateFormat("HH:mm:ss").parse(hora)

        hora = hora.replaceAll("\\D", '')

        if (hora.length() <= 4) {
            while (hora.length() < 4)
                hora = '0' + hora

            return new SimpleDateFormat('HHmm').parse(hora)
        }

        while (hora.length() < 6)
            hora = '0' + hora

        return new SimpleDateFormat('HHmmss').parse(hora)
    }

    Date parseData(Cell cell) {
        if (cell.cellType == Cell.CELL_TYPE_NUMERIC)
            return cell.dateCellValue

//        println "parseData(${cell.stringCellValue})"
        String data = cell.stringCellValue.replaceAll(~/[\\;-]/, '/').replaceAll('[^/0-9-]', '')
        assert data.length() == 10 || data.length() == 8

        if (data =~ ~/\d{2}\/\d{2}\/\d{4}/)
            return new SimpleDateFormat('dd/MM/yyyy').parse(data)

        if (data =~ ~/\d{2}\/\d{2}\/\d{2}/)
            return new SimpleDateFormat('dd/MM/yy').parse(data)

        null
    }

    void assertCellEqual(String value, Cell cell) {
        //println "${value} = ${parseString(cell)}"
        assertEquals value, parseString(cell)
    }


    static ultimo

    Object parseItem(Row row, Iterator<org.apache.poi.ss.usermodel.Row> iterator) {
        String cell0 = parseString(row.getCell(0))

        if (cell0 == null)
            return null

        if (!cell0.toUpperCase().contains('DATA') || (cell0.toUpperCase() =~ ~/\d{2}.\d{2}.*/))
            return null

        Map result = [:]

        //println "Parsing linha data"
        if (!parseLinhaData(row, result))
            return null

        row = iterator.next()
        //println "Parsing linha tipo veiculo"
        if (!parseLinhaTipoVeiculo(row, result))
            return null

        int i = 0
        while (i++ < 5) {
            row = iterator.next()

            cell0 = parseString(row.getCell(0))
            if (cell0 == null || cell0.toUpperCase().contains('CONTROL'))
                break

            assert !cell0.toUpperCase().contains('DATA')

            result.paradas = []
            parseLinhaPaciente(row, result)
        }

        if (!parseOperador(row, result))
            return null

        row = iterator.next()
        parseObs(row, result)

        ultimo = result
    }

    boolean parseLinhaData(Row row, Map result) {
        if (row.getCell(1).cellType == Cell.CELL_TYPE_BLANK)
            result.data = ultimo?.data
        else
            result.data = parseData(row.getCell(1))

        result.data != null
        true
    }

    boolean parseLinhaTipoVeiculo(Row row, Map result) {
        assertCellEqual('TIPO DE VEICULO', row.getCell(0))
        assertCellEqual('PREFIXO', row.getCell(2))
        assertCellEqual('PLACA', row.getCell(4))
        assertCellEqual('MOTORISTA', row.getCell(7))

        if (isNullOrEmpty(parseString(row.getCell(1))) && isNullOrEmpty(parseString(row.getCell(3))))
            return false

        // Ambulância
        String prefixo
        String placa

        if (row.getCell(3).cellType == Cell.CELL_TYPE_STRING &&
                !parseString(row.getCell(3)).isNumber() && parseString(row.getCell(5)).isNumber()) {
            prefixo = parseIntString(row.getCell(5))
            placa = parsePlaca(row.getCell(3).stringCellValue)
        } else {
            prefixo = parseIntString(row.getCell(3))
            placa = parsePlaca(parseIntString(row.getCell(5)))
        }

        if (prefixo.trim().empty || placa.trim().empty)
            return false

        Ambulancia ambulancia = parseAmbulancia(prefixo, placa)
        assert ambulancias.ehPrincipal(ambulancia)

        // Motorista
        String nomeMotorista = parseStringCellRange(row, 8..11)
        if (isNullOrEmpty(nomeMotorista))
            nomeMotorista = 'OUTROS'
        Motorista motorista = parseMotorista(nomeMotorista)
        assert motoristas.ehPrincipal(motorista)

        result.ambulancia = ambulancia
        result.motorista = motorista

        true
    }

    boolean parseLinhaPaciente(Row row, Map result) {
        String nomePaciente = parseString(row.getCell(1)).trim()
        String aih = parseIntString(row.getCell(8)).trim()
        String destino = semelhantes.obtenhaChave(parseStringCellRange(row, 10..12).trim())

        if (isNullOrEmpty(nomePaciente) && isNullOrEmpty(aih))
            return false

        if (isNullOrEmpty(destino))
            return false

        result.paradas.add(new ParadaPaciente(destino: destino, paciente: nomePaciente, aih: aih))

        true
    }

    boolean parseOperador(Row row, Map result) {
        String nomeControlador = parseString(row.getCell(1))

        if (nomeControlador == null)
            return false

        String kmSaida = parseIntString(row.getCell(6)).trim().replaceAll('\\D', '')
        String kmRetorno = parseIntString(row.getCell(8)).trim().replaceAll('\\D', '')

        Date horaSaida = parseHora(row.getCell(10))
        Date horaRetorno = parseHora(row.getCell(12))

//        println "$kmSaida/$kmRetorno"
//        println "$horaSaida/$horaRetorno"

        assert kmSaida.empty || kmSaida.matches(~/\d+/)
        assert kmRetorno.empty || kmRetorno.matches(~/\d+/)

        result.kmSaida = kmSaida.isNumber() ? kmSaida.toInteger() : null
        result.kmRetorno = kmRetorno.isNumber() ? kmRetorno.toInteger() : null

        result.dataSaida = result.data
        result.dataRetorno = result.data

        result.horaSaida = horaSaida
        result.horaRetorno = horaRetorno ?: horaSaida       


        if (isNullOrEmpty(nomeControlador)) {
            result.operador = Operador.findByUsuario('admin')
        } else {
            Operador novo = new Operador(nome: nomeControlador)
            Operador obtido = operadores.obtenha(nomeControlador)
            result.operador = operadores.adicione(obtido, novo)
        }

        true
    }

    boolean parseObs(Row row, Map result) {
        if (!parseString(row.getCell(0)).toUpperCase().contains('OBS'))
            return false

        result.observacoes = parseString(row.getCell(1))
    }

    List importeRegistros(FileInputStream is) {
        Workbook wb = new HSSFWorkbook(is)

        List itens = []
        (1..wb.numberOfSheets).each {
            itens.addAll(parseSheet(wb.getSheetAt(it - 1)))
        }

        itens
    }

    boolean isNullOrEmpty(String texto) {
        texto == null || texto.replaceAll('\\s', '').empty
    }

    def assertEquals(a, b) {
        assert a == b
    }
}
