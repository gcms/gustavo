<%@ page import="br.gov.go.saude.hugo.ambulancia.Ambulancia" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="ambulancia.edit" default="Edit Ambulancia"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:link class="list" action="list"><g:message code="ambulancia.list" default="Ambulancia List"/></g:link></span>
  <span class="menuButton"><g:link class="create" action="create"><g:message code="ambulancia.new" default="New Ambulancia"/></g:link></span>
</div>
<div class="body">
  <h1><g:message code="ambulancia.edit" default="Edit Ambulancia"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:hasErrors bean="${ambulanciaInstance}">
    <div class="errors">
      <g:renderErrors bean="${ambulanciaInstance}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form method="post">
    <g:hiddenField name="id" value="${ambulanciaInstance?.id}"/>
    <g:hiddenField name="version" value="${ambulanciaInstance?.version}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="prefixo"><g:message code="ambulancia.prefixo" default="Prefixo"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: ambulanciaInstance, field: 'prefixo', 'errors')}">
            <g:textField name="prefixo" value="${fieldValue(bean: ambulanciaInstance, field: 'prefixo')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="placa"><g:message code="ambulancia.placa" default="Placa"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: ambulanciaInstance, field: 'placa', 'errors')}">
            <g:textField name="placa" value="${fieldValue(bean: ambulanciaInstance, field: 'placa')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="descricao"><g:message code="ambulancia.descricao" default="Descricao"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: ambulanciaInstance, field: 'descricao', 'errors')}">
            <g:textArea name="descricao" value="${fieldValue(bean: ambulanciaInstance, field: 'descricao')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="disponivel"><g:message code="ambulancia.disponivel" default="Disponivel"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: ambulanciaInstance, field: 'disponivel', 'errors')}">
            <g:checkBox name="disponivel" value="${ambulanciaInstance?.disponivel}"/>
          </td>
        </tr>

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button"><g:actionSubmit class="save" action="update" value="${message(code: 'update', 'default': 'Update')}"/></span>
      <span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'delete', 'default': 'Delete')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
