
<%@ page import="br.gov.go.saude.hugo.ambulancia.Viagem" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <title><g:message code="viagem.create" default="Create Viagem" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home" /></a></span>
            <span class="menuButton"><g:link class="list" action="list"><g:message code="viagem.list" default="Viagem List" /></g:link></span>
        </div>
        <div class="body">
            <h1><g:message code="viagem.create" default="Create Viagem" /></h1>
            <g:if test="${flash.message}">
            <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}" /></div>
            </g:if>
            <g:hasErrors bean="${viagemInstance}">
            <div class="errors">
                <g:renderErrors bean="${viagemInstance}" as="list" />
            </div>
            </g:hasErrors>
            <g:form action="save" method="post" >
                <div class="dialog">
                    <table>
                        <tbody>
                        
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="motorista"><g:message code="viagem.motorista" default="Motorista" />:</label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'motorista', 'errors')}">
                                    <g:select name="motorista.id" from="${motoristas}" optionKey="id" value="${viagemInstance?.motorista?.id}"  />

                                </td>
                            </tr>
                        
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="ambulancia"><g:message code="viagem.ambulancia" default="Ambulancia" />:</label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'ambulancia', 'errors')}">
                                    <g:select name="ambulancia.id" from="${ambulancias}" optionKey="id" value="${viagemInstance?.ambulancia?.id}"  />

                                </td>
                            </tr>
                        
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="horaSaida"><g:message code="viagem.horaSaida" default="Hora Saida" />:</label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'horaSaida', 'errors')}">
                                    <g:datePicker name="horaSaida" value="${viagemInstance?.horaSaida}"  />

                                </td>
                            </tr>
                        
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="kmSaida"><g:message code="viagem.kmSaida" default="Km Saida" />:</label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'kmSaida', 'errors')}">
                                    <g:textField name="kmSaida" value="${fieldValue(bean: viagemInstance, field: 'kmSaida')}" />

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
                                    <label for="destino"><g:message code="viagem.destino" default="Destino" />:</label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'destino', 'errors')}">
                                    <g:textField name="destino" value="${fieldValue(bean: viagemInstance, field: 'destino')}" />

                                </td>
                            </tr>
                        
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="observacoes"><g:message code="viagem.observacoes" default="Observacoes" />:</label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: viagemInstance, field: 'observacoes', 'errors')}">
                                    <g:textField name="observacoes" value="${fieldValue(bean: viagemInstance, field: 'observacoes')}" />

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
                    <span class="button"><g:submitButton name="create" class="save" value="${message(code: 'create', 'default': 'Create')}" /></span>
                </div>
            </g:form>
        </div>
    </body>
</html>
