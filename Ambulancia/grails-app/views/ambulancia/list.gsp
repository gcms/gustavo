<%@ page import="br.gov.go.saude.hugo.ambulancia.Ambulancia" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="ambulancia.list" default="Ambulancia List"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="create" action="create"><g:message code="ambulancia.new" default="New Ambulancia"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="ambulancia.list" default="Ambulancia List"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <div class="list">
    <table>
      <thead>
      <tr>

        <g:sortableColumn property="id" title="Id" titleKey="ambulancia.id"/>

        <g:sortableColumn property="prefixo" title="Prefixo" titleKey="ambulancia.prefixo"/>

        <g:sortableColumn property="placa" title="Placa" titleKey="ambulancia.placa"/>

        <g:sortableColumn property="disponivel" title="Disponivel" titleKey="ambulancia.disponivel"/>

      </tr>
      </thead>
      <tbody>
      <g:each in="${ambulanciaInstanceList}" status="i" var="ambulanciaInstance">
        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

          <td><g:linkIfAccess action="show" id="${ambulanciaInstance.id}">${ambulanciaInstance.id}</g:linkIfAccess></td>

          <td>${fieldValue(bean: ambulanciaInstance, field: "prefixo")}</td>

          <td>${fieldValue(bean: ambulanciaInstance, field: "placa")}</td>

          <td>${ambulanciaInstance?.disponivel ? 'Sim' : 'Não'}</td>

        </tr>
      </g:each>
      </tbody>
    </table>
  </div>
  <div class="paginateButtons">
    <g:paginate total="${ambulanciaInstanceTotal}"/>
  </div>
</div>
</body>
</html>
