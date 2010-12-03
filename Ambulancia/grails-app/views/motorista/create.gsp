<%@ page import="br.gov.go.saude.hugo.ambulancia.Motorista" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="motorista.create" default="Create Motorista"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="list" action="list"><g:message code="motorista.list" default="Motorista List"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="motorista.create" default="Create Motorista"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:hasErrors bean="${motoristaInstance}">
    <div class="errors">
      <g:renderErrors bean="${motoristaInstance}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form action="save" method="post">
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="nome"><g:message code="motorista.nome" default="Nome"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: motoristaInstance, field: 'nome', 'errors')}">
            <g:textField name="nome" value="${fieldValue(bean: motoristaInstance, field: 'nome')}"/>
          </td>
        </tr>
        
        <tr class="prop">
          <td valign="top" class="name">
            <label for="telefone"><g:message code="motorista.telefone" default="Telefone"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: motoristaInstance, field: 'telefone', 'errors')}">
            <g:textField name="telefone" value="${fieldValue(bean: motoristaInstance, field: 'telefone')}"/>
          </td>
        </tr>

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button"><g:submitButton name="create" class="save" value="${message(code: 'create', 'default': 'Create')}"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
