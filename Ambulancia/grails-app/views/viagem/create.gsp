<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.create" default="Create Viagem"/></title>
  <script type="text/javascript">
    $(document).ready(function() {
      $('#dataSaida').datepicker({
        constrainInput: true,
        defaultDate: new Date(${viagemInstance?.dataSaida.time}),
        minDate: new Date(${hoje.time}),

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

  <style type="text/css">
  .x-grid3 .x-window-ml {
    padding-left: 0;
  }

  .x-grid3 .x-window-mr {
    padding-right: 0;
  }

  .x-grid3 .x-window-tl {
    padding-left: 0;
  }

  .x-grid3 .x-window-tr {
    padding-right: 0;
  }

  .x-grid3 .x-window-tc .x-window-header {
    height: 3px;
    padding: 0;
    overflow: hidden;
  }

  .x-grid3 .x-window-mc {
    border-width: 0;
    background: #cdd9e8;
  }

  .x-grid3 .x-window-bl {
    padding-left: 0;
  }

  .x-grid3 .x-window-br {
    padding-right: 0;
  }

  .x-grid3 .x-panel-btns {
    padding: 0;
  }

  .x-grid3 .x-panel-btns td.x-toolbar-cell {
    padding: 3px 3px 0;
  }

  .x-box-inner {
    zoom: 1;
  }

  .icon-user-add {
    background-image: url(${resource(dir: 'js/ext-3.3.0/examples/shared', file: 'icons/fam/user_add.gif')}) !important;
  }

  .icon-user-delete {
    background-image: url(${resource(dir: 'js/ext-3.3.0/examples/shared', file: 'icons/fam/user_delete.gif')}) !important;
  }
  </style>


  <script type="text/javascript">
    $(document).ready(function() {
      $('#horaSaida').timeEntry({
        show24Hours: true,
        spinnerImage: "${createLinkTo(dir: 'css', file: 'spinnerDefault.png')}"
      });

    });
  </script>

  <script type="text/javascript" src="${createLinkTo(dir: 'js', file: 'paciente-editor.js')}"></script>

</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:link class="list" action="list"><g:message code="viagem.list" default="Viagem List"/></g:link></span>
</div>
<div class="body">
  <h1><g:message code="viagem.create" default="Create Viagem"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:hasErrors bean="${viagemInstance}">
    <div class="errors">
      <g:renderErrors bean="${viagemInstance}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form action="save" method="post">

    <g:each var="paciente" in="${viagemInstance?.pacientes}" status="i">
      <g:hiddenField name="pacientes[${i}]" value="${paciente}"/>
    </g:each>

    <div class="dialog">
      <table>
        <tbody id="main">

        <tr class="prop">
          <td valign="top" class="name">
            <label for="motorista"><g:message code="viagem.motorista" default="Motorista"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'motorista', 'errors')}">
            <g:select name="motorista.id" from="${motoristas}" optionKey="id" value="${viagemInstance?.motorista?.id}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'ambulancia', 'errors')}">
            <g:select name="ambulancia.id" from="${ambulancias}" optionKey="id" value="${viagemInstance?.ambulancia?.id}"/>

          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataSaida"><g:message code="viagem.dataSaida" default="Data Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'dataSaida', 'errors')}">
            <input id="dataSaida" name="dataSaida" type="text" value="${formatDate(date: viagemInstance?.dataSaida, formatName: 'default.date.format')}" readonly="readonly">
            %{--<input type="text" readonly="readonly" id="dataSaida" name="dataSaida"--}%
            %{--value="${formatDate(date: viagemInstance?.horaSaida, format: 'dd/MM/yyyy')}" />--}%

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaSaida"><g:message code="viagem.horaSaida" default="Hora Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'horaSaida', 'errors')}">
            <input id="horaSaida" name="horaSaida" type="text" value="${formatDate(date: viagemInstance?.horaSaida, formatName: 'default.time.format')}"/>
            %{--<g:datePicker name="horaSaida" value="${viagemInstance?.horaSaida}"  />--}%

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmSaida"><g:message code="viagem.kmSaida" default="Km Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'kmSaida', 'errors')}">
            <g:textField name="kmSaida" value="${viagemInstance?.kmSaida}"/>

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
        %{----}%
        %{--<tr class="prop">--}%
        %{--<td valign="top" class="name">--}%
        %{--<label for="kmRetorno"><g:message code="viagem.kmRetorno" default="Km Retorno" />:</label>--}%
        %{--</td>--}%
        %{--<td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'kmRetorno', 'errors')}">--}%
        %{--<g:textField name="kmRetorno" value="${fieldValue(bean: viagemInstance, field: 'kmRetorno')}" />--}%

        %{--</td>--}%
        %{--</tr>--}%

        <tr class="prop">
          <td valign="top" class="name">
            <label for="destino"><g:message code="viagem.destino" default="Destino"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'destino', 'errors')}">
            <g:textField name="destino" value="${fieldValue(bean: viagemInstance, field: 'destino')}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="pacientes"><g:message code="viagem.pacientes" default="Pacientes"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'pacientes', 'errors')} extjs">
            <div id="paciente"></div>
          </td>
        </tr>

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
      <span class="button"><g:submitButton name="create" class="save" value="${message(code: 'create', 'default': 'Create')}"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
