<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.edit" default="Edit Viagem"/></title>

  <script type="text/javascript">
    $(document).ready(function() {
      $('#dataSaida').datepicker({
        constrainInput: true,
        defaultDate: new Date(${viagem?.dataSaida.time}),

        showOn: 'button',
        buttonImageOnly: true,
        buttonImage: "${createLinkTo(dir: 'images/skin', file: 'calendar.png')}",
      });
    });

    $(document).ready(function() {
      $('#dataRetorno').datepicker({
        constrainInput: true,
        defaultDate: new Date(${viagem?.dataRetorno.time}),

        showOn: 'button',
        buttonImageOnly: true,
        buttonImage: "${createLinkTo(dir: 'images/skin', file: 'calendar.png')}",
      });
    });
  </script>

  <script type="text/javascript">
    $(document).ready(function() {
      $('#horaSaida').timeEntry({
        show24Hours: true,
        spinnerImage: "${createLinkTo(dir: 'css', file: 'spinnerDefault.png')}"
      });
    });

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
  <span class="menuButton"><g:link class="list" action="list"><g:message code="viagem.list" default="Viagem List"/></g:link></span>
  <span class="menuButton"><g:link class="create" action="create"><g:message code="viagem.new" default="New Viagem"/></g:link></span>
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
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'motorista', 'errors')}">
            <g:select name="motorista.id" from="${br.gov.go.saude.hugo.ambulancia.Motorista.list()}" optionKey="id" value="${viagem?.motorista?.id}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'ambulancia', 'errors')}">
            <g:select name="ambulancia.id" from="${br.gov.go.saude.hugo.ambulancia.Ambulancia.list()}" optionKey="id" value="${viagem?.ambulancia?.id}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="operador"><g:message code="viagem.operador" default="Operador"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'operador', 'errors')}">
            <g:select name="operador.id" from="${br.gov.go.saude.hugo.ambulancia.Operador.list()}" optionKey="id" value="${viagem?.operador?.id}"/>

          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataSaida"><g:message code="viagem.dataSaida" default="Data Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'dataSaida', 'errors')}">
            %{--<g:datePicker name="dataSaida" value="${viagem?.dataSaida}"/>--}%
            <input id="dataSaida" name="dataSaida" type="text" value="${formatDate(date: viagem?.dataSaida, formatName: 'default.date.format')}" readonly="readonly">
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaSaida"><g:message code="viagem.horaSaida" default="Hora Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'horaSaida', 'errors')}">
            %{--<g:datePicker name="horaSaida" value="${viagem?.horaSaida}"/>--}%
            <input id="horaSaida" name="horaSaida" type="text" value="${formatDate(date: viagem?.horaSaida, formatName: 'default.time.format')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmSaida"><g:message code="viagem.kmSaida" default="Km Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'kmSaida', 'errors')}">
            <g:textField name="kmSaida" value="${viagem.kmSaida}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataRetorno"><g:message code="viagem.dataRetorno" default="Data Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'dataRetorno', 'errors')}">
            %{--<g:datePicker name="dataRetorno" value="${viagem?.dataRetorno}"/>--}%
            <input id="dataRetorno" name="dataRetorno" type="text" value="${formatDate(date: viagem?.dataRetorno, formatName: 'default.date.format')}" readonly="readonly">
          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaRetorno"><g:message code="viagem.horaRetorno" default="Hora Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'horaRetorno', 'errors')}">
            %{--<g:datePicker name="horaRetorno" value="${viagem?.horaRetorno}" noSelection="['': '']"/>--}%
            <input id="horaRetorno" name="horaRetorno" type="text" value="${formatDate(date: viagem?.horaRetorno, formatName: 'default.time.format')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmRetorno"><g:message code="viagem.kmRetorno" default="Km Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'kmRetorno', 'errors')}">
            <g:textField name="kmRetorno" value="${viagem.kmRetorno}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="distancia"><g:message code="viagem.distancia" default="Distancia"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'distancia', 'errors')}">
            <g:textField name="distancia" value="${fieldValue(bean: viagem, field: 'distancia')}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="observacoes"><g:message code="viagem.observacoes" default="Observacoes"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'observacoes', 'errors')}">
            <g:textArea name="observacoes" rows="5" cols="40" value="${fieldValue(bean: viagem, field: 'observacoes')}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="paradas"><g:message code="viagem.paradas" default="Paradas"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'paradas', 'errors')}">
          %{--<g:select name="paradas"--}%
          %{--from="${br.gov.go.saude.hugo.ambulancia.Parada.list()}"--}%
          %{--size="5" multiple="yes" optionKey="id"--}%
          %{--value="${viagem?.paradas}"/>--}%

            <g:each var="parada" in="${viagem?.paradas}">
              <g:set var="classe" value="${parada.realClass.name.tokenize('\\.').last()}"/>
              <p>
                <g:render template="${classe.uncapitalize()}" model="[parada: parada]"/>
              </p>
            </g:each>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="retornou"><g:message code="viagem.retornou" default="Retornou"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'retornou', 'errors')}">
            <g:checkBox name="retornou" value="${viagem?.retornou}"/>

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
