<%@ page import="br.gov.go.saude.hugo.ambulancia.ParadaPaciente; br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.list" default="Viagem List"/></title>
</head>
<body>
<div class="nav">
  %{--<span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home" /></a></span>--}%
  <span class="menuButton"><g:linkIfAccess class="create" action="create"><g:message code="viagem.new" default="New Viagem"/></g:linkIfAccess></span>

  <span class="menuButton"><g:linkIfAccess class="list" controller="viagem" action="list"><g:message code="viagem.list" default="Viagem List"/></g:linkIfAccess></span>
  <span class="menuButton"><g:linkIfAccess class="list" controller="motorista" action="list"><g:message code="motorista.list" default="Motorista List"/></g:linkIfAccess></span>
  <span class="menuButton"><g:linkIfAccess class="list" controller="ambulancia" action="list"><g:message code="ambulancia.list" default="Ambulancia List"/></g:linkIfAccess></span>
  <span class="menuButton"><g:linkIfAccess class="list" controller="operador" action="list"><g:message code="operador.list" default="Operador List"/></g:linkIfAccess></span>

  <span class="menuButton"><g:linkIfAccess class="logout" controller="logout"><g:message code="login.logout" default="Logout"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="viagem.list.transito" default="Viagem List"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <div class="list">
    <table>
      <thead>
      <tr>

        <g:sortableColumn property="id" title="Id" titleKey="viagem.id"/>

        <th><g:message code="viagem.ambulancia" default="Ambulancia"/></th>

        <th><g:message code="viagem.motorista" default="Motorista"/></th>

        <th><g:message code="viagem.operador" default="Operador"/></th>

        <th><g:message code="viagem.destino" default="Destinos"/></th>

        <th><g:message code="viagem.paciente" default="Pacientes"/></th>

        <g:sortableColumn property="dataSaida" title="Data Saida" titleKey="viagem.dataSaida"/>

        <g:sortableColumn property="horaSaida" title="Hora Saida" titleKey="viagem.horaSaida"/>

        <g:sortableColumn property="kmSaida" title="Km Saida" titleKey="viagem.kmSaida"/>

        <th><g:message code="viagem.acao" default="Retorno"/></th>

      </tr>
      </thead>
      <tbody>
      <g:each in="${viagens}" status="i" var="viagem">
        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

          <td><g:linkIfAccess action="show" id="${viagem.id}">${viagem.id}</g:linkIfAccess></td>

          <td>${fieldValue(bean: viagem, field: "ambulancia")}</td>

          <td>${fieldValue(bean: viagem, field: "motorista")}</td>

          <td>${fieldValue(bean: viagem, field: "operador")}</td>

          <td>
            <g:each var="parada" in="${viagem.paradas}">
              <p>${parada.destino}</p>
            </g:each>
          </td>

          <td>
            <g:each var="parada" in="${viagem.paradas}">
              <g:if test="${ParadaPaciente.isAssignableFrom(parada.realClass)}">
                <p>${parada?.paciente}</p>
              </g:if>
            </g:each>
          </td>

          <td><g:formatDate date="${viagem.dataSaida}" formatName="default.date.format"/></td>

          <td><g:formatDate date="${viagem.horaSaida}" formatName="default.time.format"/></td>

          <td>${fieldValue(bean: viagem, field: "kmSaida")}</td>

          <td style="text-align: center;">
            <g:linkIfAccess action="edit" id="${viagem.id}">
              <img src="${resource(dir: 'images/skin', file: 'next.png')}" alt="Registrar retorno" border="0" height="12pt"/>
            </g:linkIfAccess>
          </td>
        </tr>
      </g:each>
      </tbody>
    </table>
  </div>
  <div class="paginateButtons">
    <g:paginate total="${viagens.totalCount}"/>
  </div>
</div>
</body>
</html>
