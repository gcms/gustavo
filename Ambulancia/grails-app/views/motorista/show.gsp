<%@ page import="br.gov.go.saude.hugo.ambulancia.Motorista" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="motorista.show" default="Show Motorista"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="list" action="list"><g:message code="motorista.list" default="Motorista List"/></g:linkIfAccess></span>
  <span class="menuButton"><g:linkIfAccess class="create" action="create"><g:message code="motorista.new" default="New Motorista"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="motorista.show" default="Show Motorista"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:form>
    <g:hiddenField name="id" value="${motoristaInstance?.id}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="motorista.id" default="Id"/>:</td>

          <td valign="top" class="value">${motoristaInstance.id}</td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="motorista.nome" default="Nome"/>:</td>

          <td valign="top" class="value">${fieldValue(bean: motoristaInstance, field: "nome")}</td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="motorista.telefone" default="Telefone"/>:</td>

          <td valign="top" class="value">${fieldValue(bean: motoristaInstance, field: "telefone")}</td>
        </tr>

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button"><g:actionSubmitIfAccess class="edit" action="edit" value="${message(code: 'edit', 'default': 'Edit')}"/></span>
      <span class="button"><g:actionSubmitIfAccess class="delete" action="delete" value="${message(code: 'delete', 'default': 'Delete')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
