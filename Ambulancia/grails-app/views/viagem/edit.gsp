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
        defaultDate: new Date(${viagemInstance?.dataRetorno?.time}),
        minDate: new Date(${viagemInstance?.dataSaida.time}),

        showOn: 'button',
        buttonImageOnly: true,
        buttonImage: "${createLinkTo(dir: 'css/custom-theme/images', file: 'icons-mini-calendar.gif')}"
      });
    });
  </script>

  <style type="text/css">
  div.ui-datepicker {
    font-size: 14px;
  }
  </style>

  <script type="text/javascript">
    $(document).ready(function() {
      $('#horaRetorno').timeEntry({
        show24Hours: true
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
  <g:hasErrors bean="${viagemInstance}">
    <div class="errors">
      <g:renderErrors bean="${viagemInstance}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form method="post">
    <g:hiddenField name="id" value="${viagemInstance?.id}"/>
    <g:hiddenField name="version" value="${viagemInstance?.version}"/>
    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="motorista"><g:message code="viagem.motorista" default="Motorista"/>:</label>
          </td>
          <td valign="top" class="value">
            ${viagemBanco?.motorista}
            %{--<g:select name="motorista.id" from="${br.gov.go.saude.hugo.ambulancia.Motorista.list()}" optionKey="id" value="${viagemBanco?.motorista?.id}"  />--}%
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemBanco, field: 'ambulancia', 'errors')}">
            ${viagemBanco?.ambulancia}
            %{--<g:select name="ambulancia.id" from="${br.gov.go.saude.hugo.ambulancia.Ambulancia.list()}" optionKey="id" value="${viagemBanco?.ambulancia?.id}"  />--}%
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="operador"><g:message code="viagem.operador" default="Operador"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemBanco, field: 'operador', 'errors')}">
            %{--<g:select name="operador.id" from="${br.gov.go.saude.hugo.ambulancia.Operador.list()}" optionKey="id" value="${viagemBanco?.operador?.id}"/>--}%
            ${viagemBanco?.operador}
          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaSaida"><g:message code="viagem.horaSaida" default="Hora Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'horaSaida', 'errors')}">
            ${formatDate(date: viagemInstance?.horaSaida, formatName: 'default.dateTime.format')}
            %{--<g:datePicker name="horaSaida" value="${viagemInstance?.horaSaida}"/>--}%
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmSaida"><g:message code="viagem.kmSaida" default="Km Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'kmSaida', 'errors')}">
            ${viagemInstance?.kmSaida}
            %{--<g:textField name="kmSaida" value="${fieldValue(bean: viagemInstance, field: 'kmSaida')}"/>--}%
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataRetorno"><g:message code="viagem.dataRetorno" default="Data Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'horaRetorno', 'errors')}">
            <input id="dataRetorno" name="dataRetorno" type="text" value="${formatDate(date: viagemInstance?.dataRetorno, formatName: 'default.date.format')}" readonly="readonly">
            %{--<input type="text" readonly="readonly" id="dataRetorno" name="dataRetorno"--}%
            %{--value="${formatDate(date: viagemInstance?.horaRetorno, format: 'dd/MM/yyyy')}" />--}%

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaRetorno"><g:message code="viagem.horaRetorno" default="Hora Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'horaRetorno', 'errors')}">
            <input id="horaRetorno" name="horaRetorno" type="text" value="${formatDate(date: viagemInstance?.horaRetorno, formatName: 'default.time.format')}"/>
            %{--<g:datePicker name="horaRetorno" value="${viagemInstance?.horaRetorno}"  />--}%

          </td>
        </tr>


        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="horaRetorno"><g:message code="viagem.horaRetorno" default="Hora Retorno" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'horaRetorno', 'errors')}">--}%
        %{--<g:datePicker name="horaRetorno" value="${viagemInstance?.horaRetorno}" noSelection="['': '']" />--}%

        %{--</td>--}%
        %{--</tr>--}%

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmRetorno"><g:message code="viagem.kmRetorno" default="Km Retorno"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'kmRetorno', 'errors')}">
            <g:textField name="kmRetorno" value="${fieldValue(bean: viagemInstance, field: 'kmRetorno')}"/>
          </td>
        </tr>

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="destino"><g:message code="viagem.destino" default="Destino" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'destino', 'errors')}">--}%
        %{--<g:textField name="destino" value="${fieldValue(bean: viagemInstance, field: 'destino')}" />--}%

        %{--</td>--}%
        %{--</tr>--}%
        %{----}%
        <tr class="prop">
          <td valign="top" class="name">
            <label for="observacoes"><g:message code="viagem.observacoes" default="Observacoes"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'observacoes', 'errors')}">
            <g:textArea name="observacoes" value="${fieldValue(bean: viagemInstance, field: 'observacoes')}"/>

          </td>
        </tr>

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="pacientes"><g:message code="viagem.pacientes" default="Pacientes" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'pacientes', 'errors')}">--}%
        %{----}%

        %{--</td>--}%
        %{--</tr>--}%

        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="retornou"><g:message code="viagem.retornou" default="Retornou" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'retornou', 'errors')}">--}%
        %{--<g:checkBox name="retornou" value="${viagemInstance?.retornou}" />--}%

        %{--</td>--}%
        %{--</tr>--}%

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
