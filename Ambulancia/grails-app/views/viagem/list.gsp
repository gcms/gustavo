<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.list" default="Viagem List"/></title>

  <script type="text/javascript">
    $(document).ready(function() {
      $('#dataInicio').datepicker({
        constrainInput: true,
        defaultDate: new Date(${dataInicio.time}),
        showOn: 'button',
        buttonImageOnly: true,
        buttonImage: "${createLinkTo(dir: 'images/skin', file: 'calendar.png')}",
        changeYear: true
      });
    });

    $(document).ready(function() {
      $('#dataFim').datepicker({
        constrainInput: true,
        defaultDate: new Date(${dataFim.time}),
        showOn: 'button',
        buttonImageOnly: true,
        buttonImage: "${createLinkTo(dir: 'images/skin', file: 'calendar.png')}",
        changeYear: true
      });
    });
  </script>
  <style type="text/css">
  div.ui-datepicker {
    font-size: 14px;
  }
  </style>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="create" action="create"><g:message code="viagem.new" default="New Viagem"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="viagem.list.retornou" default="Viagem List"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>

  <g:form action="list" method="get">
    <div class="dialog">
      <table>
        <tbody id="main">

        <tr class="prop">
          <td valign="top" class="name">
            <label for="motorista"><g:message code="viagem.motorista" default="Motorista"/>:</label>
          </td>
          <td valign="top" class="value">
            <g:select name="motorista.id" from="${motoristas}" optionKey="id" value="${motorista?.id}"
                    noSelection="${[0:'Todos']}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</label>
          </td>
          <td valign="top" class="value">
            <g:select name="ambulancia.id" from="${ambulancias}" optionKey="id" value="${ambulancia?.id}"
                    noSelection="${[0:'Todos']}"/>
          </td>
        </tr>
        
        <tr class="prop">
          <td valign="top" class="name">
            <label for="destino"><g:message code="viagem.parada.destino" default="Destino"/>:</label>
          </td>
          <td valign="top" class="value">
            <g:select name="destino" from="${destinos}" value="${destino}"
                    noSelection="${[0:'Todos']}"/>
          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataInicio"><g:message code="viagem.dataInicio" default="Data Inicio"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'dataInicio', 'errors')}">
            <input id="dataInicio" name="dataInicio" type="text" value="${formatDate(date: dataInicio, formatName: 'default.date.format')}" readonly="readonly">
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataFim"><g:message code="viagem.dataFim" default="Data Fim"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'dataFim', 'errors')}">
            <input id="dataFim" name="dataFim" type="text" value="${formatDate(date: dataFim, formatName: 'default.date.format')}" readonly="readonly">
          </td>
        </tr>

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button">
        <g:submitButton name="filter" class="filter" value="${message(code: 'viagem.filter', 'default': 'Filter')}"/>
      </span>
      <span class="button">
        %{--<g:submitButton name="print" class="print" value="${message(code: 'print', 'default': 'Print')}" onclick="document.forms[0].action = 'print'; document.forms[0].target= '_blank'; document.forms[0].submit();"/>--}%
        <g:actionSubmitIfAccess name="print" class="print" value="${message(code: 'viagem.print', 'default': 'Print')}" action="print"/>
      </span>
    </div>
  </g:form>

  <p>&nbsp;</p>

  <div class="dialog">
    <table>
      <tbody>

      <tr class="prop">
        <td valign="top" class="name"><g:message code="distancia.total" default="Distância total"/>:</td>

        <td valign="top" class="value">${distanciaTotal ?: 0} km</td>
      </tr>

      %{--<tr class="prop">--}%
      %{--<td valign="top" class="name"><g:message code="tempo.total" default="Tempo total"/>:</td>--}%

      %{--<td valign="top" class="value"></td>--}%
      %{--</tr>--}%
      </tbody>
    </table>
  </div>


  <div class="list">
    <table>
      <thead>
      <tr>

        <g:sortableColumn property="id" title="Id" titleKey="viagem.id"/>

        <th><g:message code="viagem.ambulancia" default="Ambulancia"/></th>

        <th><g:message code="viagem.motorista" default="Motorista"/></th>

        %{--<th><g:message code="viagem.operador" default="Operador" /></th>--}%

        <th><g:message code="viagem.destino" default="Destinos"/></th>

        <g:sortableColumn property="dataSaida" title="Data Saida" titleKey="viagem.dataSaida"/>
        <g:sortableColumn property="horaSaida" title="Hora Saida" titleKey="viagem.horaSaida"/>

        <g:sortableColumn property="dataRetorno" title="Data Retorno" titleKey="viagem.dataRetorno"/>
        <g:sortableColumn property="horaRetorno" title="Hora Retorno" titleKey="viagem.horaRetorno"/>

        <g:sortableColumn property="kmSaida" title="Km Saida" titleKey="viagem.kmSaida"/>
        <g:sortableColumn property="kmRetorno" title="Km Retorno" titleKey="viagem.kmRetorno"/>
        <g:sortableColumn property="distancia" title="Distancia" titleKey="viagem.distancia"/>

      </tr>
      </thead>
      <tbody>
      <g:each in="${viagens}" status="i" var="viagemInstance">
        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

          <td><g:linkIfAccess action="show" id="${viagemInstance.id}">${fieldValue(bean: viagemInstance, field: "id")}</g:linkIfAccess></td>

          <td>${fieldValue(bean: viagemInstance, field: "ambulancia")}</td>

          <td>${fieldValue(bean: viagemInstance, field: "motorista")}</td>


          %{--<td>${fieldValue(bean: viagemInstance, field: "operador")}</td>--}%

          <td>
            <g:each var="parada" in="${viagemInstance.paradas}">
              <p>${parada.destino}</p>
            </g:each>
          </td>

          <td><g:formatDate date="${viagemInstance.dataSaida}" formatName="default.date.format"/></td>
          <td><g:formatDate date="${viagemInstance.horaSaida}" formatName="default.time.format"/></td>

          <td><g:formatDate date="${viagemInstance.dataRetorno}" formatName="default.date.format"/></td>
          <td><g:formatDate date="${viagemInstance.horaRetorno}" formatName="default.time.format"/></td>

          <td>${fieldValue(bean: viagemInstance, field: "kmSaida")}</td>
          <td>${fieldValue(bean: viagemInstance, field: "kmRetorno")}</td>
          <td>${fieldValue(bean: viagemInstance, field: "distancia")}</td>

        </tr>
      </g:each>
      </tbody>
    </table>
  </div>
  <div class="paginateButtons">
    <g:paginate total="${viagens.totalCount}"
            params="${[dataInicio: formatDate(date: dataInicio, formatName: 'default.date.format'),
                       dataFim: formatDate(date: dataFim, formatName: 'default.date.format'),
                       'motorista.id': (motorista?.id ?: 0),
                       'ambulancia.id': (ambulancia?.id ?: 0)]}"/>
  </div>
</div>
</body>
</html>
