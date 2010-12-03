<%@ page import="br.gov.go.saude.hugo.ambulancia.Operador" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="operador.edit" default="Edit Operador"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="list" action="list"><g:message code="operador.list" default="Operador List"/></g:linkIfAccess></span>
  <span class="menuButton"><g:linkIfAccess class="create" action="create"><g:message code="operador.new" default="New Operador"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="operador.edit" default="Edit Operador"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:hasErrors bean="${operadorInstance}">
    <div class="errors">
      <g:renderErrors bean="${operadorInstance}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form method="post">
    <g:hiddenField name="id" value="${operadorInstance?.id}"/>
    <g:hiddenField name="version" value="${operadorInstance?.version}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="nome"><g:message code="operador.nome" default="Nome"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'nome', 'errors')}">
            <g:textField name="nome" value="${fieldValue(bean: operadorInstance, field: 'nome')}"/>
          </td>
        </tr>
        
        <tr class="prop">
          <td valign="top" class="name">
            <label for="telefone"><g:message code="operador.telefone" default="Telefone"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'telefone', 'errors')}">
            <g:textField name="telefone" value="${fieldValue(bean: operadorInstance, field: 'telefone')}"/>
          </td>
        </tr>        

        <tr class="prop">
          <td valign="top" class="name">
            <label for="usuario"><g:message code="operador.usuario" default="Usuario"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'usuario', 'errors')}">
            <g:textField name="usuario" value="${fieldValue(bean: operadorInstance, field: 'usuario')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="senha"><g:message code="operador.senha" default="Senha"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'senha', 'errors')}">
            <g:passwordField name="senha" value=""/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="senha"><g:message code="operador.senhaConfirmada" default="Confirmação"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'senha', 'errors')}">
            <g:passwordField name="senhaConfirmada" value=""/>
          </td>
        </tr>

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="accountExpired"><g:message code="operador.accountExpired" default="Account Expired" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'accountExpired', 'errors')}">--}%
        %{--<g:checkBox name="accountExpired" value="${operadorInstance?.accountExpired}" />--}%

        %{--</td>--}%
        %{--</tr>--}%
        %{----}%
        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="accountLocked"><g:message code="operador.accountLocked" default="Account Locked" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'accountLocked', 'errors')}">--}%
        %{--<g:checkBox name="accountLocked" value="${operadorInstance?.accountLocked}" />--}%

        %{--</td>--}%
        %{--</tr>--}%

        <tr class="prop">
          <td valign="top" class="name">
            <label for="enabled"><g:message code="operador.enabled" default="Enabled"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'enabled', 'errors')}">
            <g:checkBox name="enabled" value="${operadorInstance?.enabled}"/>
          </td>
        </tr>

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="passwordExpired"><g:message code="operador.passwordExpired" default="Password Expired" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'passwordExpired', 'errors')}">--}%
        %{--<g:checkBox name="passwordExpired" value="${operadorInstance?.passwordExpired}" />--}%

        %{--</td>--}%
        %{--</tr>--}%

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="authorities"><g:message code="operador.authorities" default="Authorities" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'authorities', 'errors')}">--}%
        %{----}%

        %{--</td>--}%
        %{--</tr>--}%
        %{----}%
        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="grupos"><g:message code="operador.grupos" default="Grupos" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: operadorInstance, field: 'grupos', 'errors')}">--}%
        %{----}%

        %{--</td>--}%
        %{--</tr>--}%

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button"><g:actionSubmitIfAccess class="save" action="update" value="${message(code: 'update', 'default': 'Update')}"/></span>
      <span class="button"><g:actionSubmitIfAccess class="delete" action="delete" value="${message(code: 'delete', 'default': 'Delete')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
