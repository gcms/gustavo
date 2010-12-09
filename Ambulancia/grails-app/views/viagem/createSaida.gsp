<%@ page import="grails.converters.deep.JSON; br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title><g:message code="viagem.create" default="Create Viagem"/></title>
  <script type="text/javascript">
    $(document).ready(function() {
      $('#dataSaida').datepicker({
        constrainInput: true,
        defaultDate: new Date(${viagem?.dataSaida.time}),
        minDate: new Date(${hoje.time}),

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
  </script>

  <script type="text/javascript" src="${resource(dir: 'js', file: 'lista-paradas.js')}"></script>

  <script type="text/javascript">
    var items = new Ambulancia.FieldSetList({ renderTo: 'paradas' });

    function adicionePaciente() {
      items.adicione(Ambulancia.FieldSetList.TiposParadas['br.gov.go.saude.hugo.ambulancia.ParadaPaciente']);
    }

    function adicioneServicos() {
      items.adicione(Ambulancia.FieldSetList.TiposParadas['br.gov.go.saude.hugo.ambulancia.ParadaServicos']);
    }

    var paradas = ${viagem?.paradas as JSON};

    Ext.onReady(function() {
      Ext.QuickTips.init();

      for (var i = 0; i < paradas.length; i++) {
        var parada = paradas[i];
        items.adicione(Ambulancia.FieldSetList.TiposParadas[parada['class']], parada);
      }
    });

  </script>
  %{--<script type="text/javascript" src="${createLinkTo(dir: 'js', file: 'paciente-editor.js')}"></script>--}%

</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home"/></a></span>
  <span class="menuButton"><g:linkIfAccess class="list" action="list"><g:message code="viagem.list" default="Viagem List"/></g:linkIfAccess></span>
</div>
<div class="body">
  <h1><g:message code="viagem.create" default="Create Viagem"/></h1>
  <g:if test="${flash.message}">
    <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}"/></div>
  </g:if>
  <g:hasErrors bean="${viagem}">
    <div class="errors">
      <g:renderErrors bean="${viagem}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form action="saveSaida" method="post">

    <div class="dialog">
      <table>
        <tbody id="main">

        <tr class="prop">
          <td valign="top" class="name">
            <label for="motorista"><g:message code="viagem.motorista" default="Motorista"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'motorista', 'errors')}">
            <g:select name="motorista.id" from="${motoristas}" optionKey="id" value="${viagem?.motorista?.id}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'ambulancia', 'errors')}">
            <g:select name="ambulancia.id" from="${ambulancias}" optionKey="id" value="${viagem?.ambulancia?.id}"/>

          </td>
        </tr>


        <tr class="prop">
          <td valign="top" class="name">
            <label for="dataSaida"><g:message code="viagem.dataSaida" default="Data Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'dataSaida', 'errors')}">
            <input id="dataSaida" name="dataSaida" type="text" value="${formatDate(date: viagem?.dataSaida, formatName: 'default.date.format')}" readonly="readonly">
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="horaSaida"><g:message code="viagem.horaSaida" default="Hora Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'horaSaida', 'errors')}">
            <input id="horaSaida" name="horaSaida" type="text" value="${formatDate(date: viagem?.horaSaida, formatName: 'default.time.format')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="kmSaida"><g:message code="viagem.kmSaida" default="Km Saida"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'kmSaida', 'errors')}">
            <g:textField name="kmSaida" value="${viagem?.kmSaida}"/>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="paradas"><g:message code="viagem.paradas" default="Paradas"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'paradas', 'errors')} extjs link-list">
            <span class="list-link">
              <a href="#" onclick="adicionePaciente()">
                <img src="${resource(dir: 'images', file: 'skin/person.png')}"/>
                paciente
              </a>
            </span>

            <span class="list-link">
              <a href="#" onclick="adicioneServicos()">
                <img src="${resource(dir: 'images', file: 'skin/truck.png')}"/>
                serviços
              </a>
            </span>
            <div id="paradas"></div>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="observacoes"><g:message code="viagem.observacoes" default="Observacoes"/>:</label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: viagem, field: 'observacoes', 'errors')}">
            <g:textArea name="observacoes" value="${fieldValue(bean: viagem, field: 'observacoes')}"/>

          </td>
        </tr>

        </tbody>
      </table>
    </div>
    <div class="buttons">
      <span class="button"><g:submitButton name="create" class="save" value="${message(code: 'viagem.save', 'default': 'Create')}"/></span>
    </div>
  </g:form>
</div>
</body>
</html>
