<%@ page import="br.gov.go.saude.hugo.ambulancia.Ambulancia" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="ambulancia.show" default="Show Ambulancia"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:link class="list" action="list"><g:message code="ambulancia.list" default="Ambulancia List"/></g:link></span>
  <span class="menuButton"><g:link class="create" action="create"><g:message code="ambulancia.new" default="New Ambulancia"/></g:link></span>
</div>
<div class="body">
  <h1><g:message code="ambulancia.show" default="Show Ambulancia"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:form>
    <g:hiddenField name="id" value="${ambulanciaInstance?.id}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="ambulancia.id" default="Id"/>:</td>
          <td valign="top" class="value">${fieldValue(bean: ambulanciaInstance, field: "id")}</td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name"><g:message code="ambulancia.prefixo" default="Prefixo"/>:</td>
          <td valign="top" class="value">${fieldValue(bean: ambulanciaInstance, field: "prefixo")}</td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="ambulancia.placa" default="Placa"/>:</td>
          <td valign="top" class="value">${fieldValue(bean: ambulanciaInstance, field: "placa")}</td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="ambulancia.descricao" default="Descricao"/>:</td>
          <td valign="top" class="value">${fieldValue(bean: ambulanciaInstance, field: "descricao")}</td>
        </tr>
        
        <tr class="prop">
          <td valign="top" class="name"><g:message code="ambulancia.disponivel" default="Disponivel"/>:</td>
          <td valign="top" class="value">${ambulanciaInstance?.disponivel ? 'Sim' : 'Não'}</td>
        </tr>

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button"><g:actionSubmit class="edit" action="edit" value="${message(code: 'edit', 'default': 'Edit')}"/></span>
      <span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'delete', 'default': 'Delete')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
