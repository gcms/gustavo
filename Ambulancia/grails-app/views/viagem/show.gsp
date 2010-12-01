<%@ page import="grails.converters.deep.JSON; br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.show" default="Show Viagem"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:link class="list" action="list"><g:message code="viagem.list" default="Viagem List"/></g:link></span>
  <span class="menuButton"><g:link class="create" action="create"><g:message code="viagem.new" default="New Viagem"/></g:link></span>
</div>
<div class="body">
  <h1><g:message code="viagem.show" default="Show Viagem"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:form>
    <g:hiddenField name="id" value="${viagem?.id}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.id" default="Id"/>:</td>

          <td valign="top" class="value">${fieldValue(bean: viagem, field: "id")}</td>

        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.motorista" default="Motorista"/>:</td>

          <td valign="top" class="value"><g:link controller="motorista" action="show" id="${viagem?.motorista?.id}">${viagem?.motorista?.encodeAsHTML()}</g:link></td>

        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</td>

          <td valign="top" class="value"><g:link controller="ambulancia" action="show" id="${viagem?.ambulancia?.id}">${viagem?.ambulancia?.encodeAsHTML()}</g:link></td>

        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.operador" default="Operador"/>:</td>

          <td valign="top" class="value"><g:link controller="operador" action="show" id="${viagem?.operador?.id}">${viagem?.operador?.encodeAsHTML()}</g:link></td>

        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.horaSaida" default="Hora Saida"/>:</td>

          <td valign="top" class="value"><g:formatDate date="${viagem?.horaSaida}" formatName="default.dateTime.format"/></td>

        </tr>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.kmSaida" default="Km Saida"/>:</td>

          <td valign="top" class="value">${fieldValue(bean: viagem, field: "kmSaida")}</td>

        </tr>

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name"><g:message code="viagem.horaRetorno" default="Hora Retorno" />:</td>--}%
        %{----}%
        %{--<td valign="top" class="value"><g:formatDate date="${viagem?.horaRetorno}" /></td>--}%
        %{----}%
        %{--</tr>--}%
        %{----}%
        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name"><g:message code="viagem.kmRetorno" default="Km Retorno" />:</td>--}%
        %{----}%
        %{--<td valign="top" class="value">${fieldValue(bean: viagem, field: "kmRetorno")}</td>--}%
        %{----}%
        %{--</tr>--}%

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name"><g:message code="viagem.destino" default="Destino"/>:</td>--}%

        %{--<td valign="top" class="value">${fieldValue(bean: viagem, field: "destino")}</td>--}%

        %{--</tr>--}%

        <g:if test="${!viagem.paradas.empty}">
          <tr class="prop">
            <td valign="top" class="name"><g:message code="viagem.paradas" default="Paradas"/>:</td>

            <td valign="top" class="value">
              <g:each var="parada" in="${viagem?.paradas}">
                <p>
                  ${parada}
                  %{--<g:render template="${parada.className.decapitalize()}" model="[parada: parada]"/> --}%
                </p>
              </g:each>
            %{--${fieldValue(bean: viagem, field: "pacientes")}--}%
            </td>
          </tr>
        </g:if>

        <tr class="prop">
          <td valign="top" class="name"><g:message code="viagem.observacoes" default="Observacoes"/>:</td>

          <td valign="top" class="value">${fieldValue(bean: viagem, field: "observacoes")}</td>

        </tr>



        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name"><g:message code="viagem.retornou" default="Retornou" />:</td>--}%
        %{----}%
        %{--<td valign="top" class="value"><g:formatBoolean boolean="${viagem?.retornou}" /></td>--}%
        %{----}%
        %{--</tr>--}%

        </tbody>
      </table>
    </div>
    <g:if test="${!viagem.retornou}">
      <div class="buttons">
        <span class="button"><g:actionSubmit class="next" action="edit" value="${message(code: 'viagem.next', 'default': 'Edit')}"/></span>
        <span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'viagem.cancel', 'default': 'Cancel viagem')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');"/></span>
      </div>
    </g:if>
  </g:form>
</div>
</body>
</html>
