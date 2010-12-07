<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.relatorio" default="Relatório de viagens"/></title>
  <style type="text/css">
  td, th {
    border: solid black 1px;
    padding: 2pt 6pt;
  }

  th {
    background-color: #dddddd;
  }

  .table {
    padding: 0pt;
  }
  </style>
<body>

<table>
  <tr>
    <td><g:message code="viagem.operador" default="Operador"/>:</td>
    <td>${operador.nome}</td>
  </tr>
  <tr>
    <td>Data/Hora:</td>
    <td>${formatDate(date: new Date(), formatName: 'default.dateTime.format')}</td>
  </tr>
  <tr>
    <td><g:message code="viagem.motorista" default="Motorista"/>:</td>
    <td>${motorista ?: 'Todos'}</td>
  </tr>
  <tr>
    <td><g:message code="viagem.ambulancia" default="Ambulancia"/>:</td>
    <td>${ambulancia ?: 'Todas'}</td>
  </tr>
  <tr>
    <td><g:message code="viagem.destino" default="Destino"/>:</td>
    <td>${destino ?: 'Todos'}</td>
  </tr>
  <tr>
    <td>Período:</td>
    <td>${formatDate(date: dataInicio, formatName: 'default.date.format')} a ${formatDate(date: dataFim, formatName: 'default.date.format')}</td>
  </tr>

  <tr>
    <td><g:message code="distancia.total" default="Distância total"/>:</td>
    <td>${distanciaTotal} km</td>
  </tr>

  <tr>
    <td colspan="2" class="table">
      <table>
        <tr>
          <th><g:message code="viagem.ambulancia" default="Ambulancia"/></th>
          <th><g:message code="viagem.motorista" default="Motorista"/></th>

          <th><g:message code="viagem.dataSaida" default="Data Saida"/></th>
          <th><g:message code="viagem.horaSaida" default="Hora Saida"/></th>

          <th><g:message code="viagem.dataRetorno" default="Data Retorno"/></th>
          <th><g:message code="viagem.horaRetorno" default="Hora Retorno"/></th>


          <th><g:message code="viagem.kmSaida" default="Km Saida"/></th>
          <th><g:message code="viagem.kmRetorno" default="Km Retorno"/></th>
          <th><g:message code="viagem.distancia" default="Distancia"/></th>
        </tr>

        <g:each var="viagem" in="${viagens}">
          <tr>
            <td>${viagem.ambulancia.placa}</td>
            <td>${viagem.motorista.nome}</td>

            <td>${formatDate(date: viagem.dataSaida, formatName: 'default.date.format')}</td>
            <td>${formatDate(date: viagem.horaSaida, formatName: 'default.time.format')}</td>

            <td>${formatDate(date: viagem.dataRetorno, formatName: 'default.date.format')}</td>
            <td>${formatDate(date: viagem.horaRetorno, formatName: 'default.time.format')}</td>

            <td>${viagem.kmSaida}</td>
            <td>${viagem.kmRetorno}</td>
            <td>${viagem.distancia}</td>
          </tr>
        </g:each>
      </table>
    </td>
  </tr>
</table>

</body>
</html>
