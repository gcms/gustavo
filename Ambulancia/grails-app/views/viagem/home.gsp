<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.list" default="Viagem List"/></title>
</head>
<body>
<div class="nav">
  %{--<span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home" /></a></span>--}%
  <span class="menuButton"><g:link class="create" action="create"><g:message code="viagem.new" default="New Viagem"/></g:link></span>

  <span class="menuButton"><g:link class="list" controller="viagem" action="list"><g:message code="viagem.list" default="Viagem List"/></g:link></span>
  <span class="menuButton"><g:link class="list" controller="motorista" action="list"><g:message code="motorista.list" default="Motorista List"/></g:link></span>
  <span class="menuButton"><g:link class="list" controller="ambulancia" action="list"><g:message code="ambulancia.list" default="Ambulancia List"/></g:link></span>
  <sec:ifAllGranted roles="ROLE_ADMIN">
    <span class="menuButton"><g:link class="list" controller="operador" action="list"><g:message code="operador.list" default="Operador List"/></g:link></span>
  </sec:ifAllGranted>

  <span class="menuButton"><g:link class="logout" controller="logout"><g:message code="login.logout" default="Logout"/></g:link></span>
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

        %{--<g:sortableColumn property="id" title="Id" titleKey="viagem.id"/>--}%

        <th><g:message code="viagem.ambulancia" default="Ambulancia"/></th>

        <th><g:message code="viagem.motorista" default="Motorista"/></th>

        %{--<th><g:message code="viagem.operador" default="Operador" /></th>--}%

        <g:sortableColumn property="dataSaida" title="Data Saida" titleKey="viagem.dataSaida"/>

        <g:sortableColumn property="horaSaida" title="Hora Saida" titleKey="viagem.horaSaida"/>

        <g:sortableColumn property="kmSaida" title="Km Saida" titleKey="viagem.kmSaida"/>

        <th><g:message code="viagem.acao" default="Retorno"/></th>

      </tr>
      </thead>
      <tbody>
      <g:each in="${viagemInstanceList}" status="i" var="viagemInstance">
        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

          %{--<td><g:link action="show" id="${viagemInstance.id}">${fieldValue(bean: viagemInstance, field: "id")}</g:link></td>--}%

          <td>${fieldValue(bean: viagemInstance, field: "ambulancia")}</td>

          <td>${fieldValue(bean: viagemInstance, field: "motorista")}</td>


          %{--<td>${fieldValue(bean: viagemInstance, field: "operador")}</td>--}%

          <td><g:formatDate date="${viagemInstance.dataSaida}" formatName="default.date.format"/></td>

          <td><g:formatDate date="${viagemInstance.horaSaida}" formatName="default.time.format"/></td>

          <td>${fieldValue(bean: viagemInstance, field: "kmSaida")}</td>

          <td style="text-align: center;">
            <g:link action="show" id="${viagemInstance.id}">
              <img src="${resource(dir: 'images/skin', file: 'next.png')}" alt="Registrar retorno" border="0" height="12pt"/>
            </g:link>
          </td>
        </tr>
      </g:each>
      </tbody>
    </table>
  </div>
  <div class="paginateButtons">
    <g:paginate total="${viagemInstanceTotal}"/>
  </div>
</div>
</body>
</html>
