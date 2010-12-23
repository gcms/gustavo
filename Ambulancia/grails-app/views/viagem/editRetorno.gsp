<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.edit" default="Edit Viagem"/></title>

  <script type="text/javascript">
    $(document).ready(function() {
      $('#dataRetorno').datepicker({
        constrainInput: true,
        defaultDate: new Date(${viagem?.dataRetorno?.time}),
        minDate: new Date(${viagem?.dataSaida.time}),

        showOn: 'button',
        buttonImageOnly: true,
        buttonImage: "${createLinkTo(dir: 'images/skin', file: 'calendar.png')}",
      });
    });
  </script>

  <script type="text/javascript">
    $(document).ready(function() {
      $('#horaRetorno').timeEntry({
        show24Hours: true,
        spinnerImage: "${createLinkTo(dir: 'css', file: 'spinnerDefault.png')}"
      });
    });
  </script>

</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="list" action="list"><g:message code="viagem.list" default="Viagem List"/></g:linkIfAccess></span>
  <span class="menuButton"><g:linkIfAccess class="create" action="createSaida"><g:message code="viagem.new" default="New Viagem"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="viagem.edit" default="Edit Viagem"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:hasErrors bean="${viagem}">
    <div class="errors">
      <g:renderErrors bean="${viagem}" as="list"/>
    </div>
  </g:hasErrors>
  <g:if test="${flash.aviso}">
    <div class="warning">
      <ul><li>${flash.aviso}</li></ul>
    </div>
  </g:if>

  <g:form method="post">
    <g:hiddenField name="id" value="${viagem?.id}"/>
    <g:hiddenField name="version" value="${viagem?.version}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="motorista"><g:message code="viagem.motorista" default="Motorista"/>:</label>
          </td>
          <td valign="top" class="value">
            ${viagemBanco?.motorista}
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemBanco, field: 'ambulancia', 'errors')}">
            ${viagemBanco?.ambulancia}
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="operador"><g:message code="viagem.operador" default="Operador"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemBanco, field: 'operador', 'errors')}">
            ${viagemBanco?.operador}
          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaSaida"><g:message code="viagem.horaSaida" default="Hora Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'horaSaida', 'errors')}">
            ${formatDate(date: viagemBanco?.horaSaida, formatName: 'default.dateTime.format')}
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmSaida"><g:message code="viagem.kmSaida" default="Km Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'kmSaida', 'errors')}">
            ${viagemBanco?.kmSaida}
          </td>
        </tr>

        <g:if test="${!viagem.paradas.empty}">
          <tr class="prop">
            <td valign="top" class="name">
              <label for="pacientes"><g:message code="viagem.paradas" default="Paradas"/>:</label>
            </td>
            <td valign="top" class="value ${hasErrors(bean: viagem, field: 'paradas', 'errors')}">
              <g:each var="parada" in="${viagemBanco?.paradas}">
                <g:set var="classe" value="${parada.realClass.name.tokenize('\\.').last()}"/>
                <p>
                  <g:render template="${classe.uncapitalize()}" model="[parada: parada]"/>
                </p>
              </g:each>
            </td>
          </tr>
        </g:if>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataRetorno"><g:message code="viagem.dataRetorno" default="Data Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'dataRetorno', 'errors')}">
            <input id="dataRetorno" name="dataRetorno" type="text" value="${formatDate(date: viagem?.dataRetorno, formatName: 'default.date.format')}" readonly="readonly">
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaRetorno"><g:message code="viagem.horaRetorno" default="Hora Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'horaRetorno', 'errors')}">
            <input id="horaRetorno" name="horaRetorno" type="text" value="${formatDate(date: viagem?.horaRetorno, formatName: 'default.time.format')}"/>
          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmRetorno"><g:message code="viagem.kmRetorno" default="Km Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'kmRetorno', 'errors')}  ${flash.aviso ? 'warning' : ''}">
            <g:textField name="kmRetorno" value="${viagem?.kmRetorno}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="observacoes"><g:message code="viagem.observacoes" default="Observacoes"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'observacoes', 'errors')}">
            <g:textArea name="observacoes" value="${viagem?.observacoes}"/>
          </td>
        </tr>
        </tbody>
      </table>
    </div>

    <g:hiddenField name="confirme" value="${flash.aviso}"/>

    <div class="buttons">
      <span class="button"><g:actionSubmitIfAccess class="save" action="updateRetorno" value="${message(code: 'viagem.update', 'default': 'Update')}"/></span>
      <span class="button"><g:actionSubmitIfAccess class="delete" action="deleteSaida" value="${message(code: 'viagem.cancel', 'default': 'Cancel viagem')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
