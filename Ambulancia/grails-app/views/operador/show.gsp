
<%@ page import="br.gov.go.saude.hugo.ambulancia.Operador" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <title><g:message code="operador.show" default="Show Operador" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home" /></a></span>
            <span class="menuButton"><g:link class="list" action="list"><g:message code="operador.list" default="Operador List" /></g:link></span>
            <span class="menuButton"><g:link class="create" action="create"><g:message code="operador.new" default="New Operador" /></g:link></span>
        </div>
        <div class="body">
            <h1><g:message code="operador.show" default="Show Operador" /></h1>
            <g:if test="${flash.message}">
            <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}" /></div>
            </g:if>
            <g:form>
                <g:hiddenField name="id" value="${operadorInstance?.id}" />
                <div class="dialog">
                    <table>
                        <tbody>
                        
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.id" default="Id" />:</td>
                                
                                <td valign="top" class="value">${fieldValue(bean: operadorInstance, field: "id")}</td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.nome" default="Nome" />:</td>
                                
                                <td valign="top" class="value">${fieldValue(bean: operadorInstance, field: "nome")}</td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.usuario" default="Usuario" />:</td>
                                
                                <td valign="top" class="value">${fieldValue(bean: operadorInstance, field: "usuario")}</td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.senha" default="Senha" />:</td>
                                
                                <td valign="top" class="value">${fieldValue(bean: operadorInstance, field: "senha")}</td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.accountExpired" default="Account Expired" />:</td>
                                
                                <td valign="top" class="value"><g:formatBoolean boolean="${operadorInstance?.accountExpired}" /></td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.accountLocked" default="Account Locked" />:</td>
                                
                                <td valign="top" class="value"><g:formatBoolean boolean="${operadorInstance?.accountLocked}" /></td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.enabled" default="Enabled" />:</td>
                                
                                <td valign="top" class="value"><g:formatBoolean boolean="${operadorInstance?.enabled}" /></td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.passwordExpired" default="Password Expired" />:</td>
                                
                                <td valign="top" class="value"><g:formatBoolean boolean="${operadorInstance?.passwordExpired}" /></td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.authorities" default="Authorities" />:</td>
                                
                                <td valign="top" class="value">${fieldValue(bean: operadorInstance, field: "authorities")}</td>
                                
                            </tr>
                            
                            <tr class="prop">
                                <td valign="top" class="name"><g:message code="operador.grupos" default="Grupos" />:</td>
                                
                                <td valign="top" class="value">${fieldValue(bean: operadorInstance, field: "grupos")}</td>
                                
                            </tr>
                            
                        </tbody>
                    </table>
                </div>
                <div class="buttons">
                    <span class="button"><g:actionSubmit class="edit" action="edit" value="${message(code: 'edit', 'default': 'Edit')}" /></span>
                    <span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'delete', 'default': 'Delete')}" onclick="return confirm('${message(code: 'delete.confirm', 'default': 'Are you sure?')}');" /></span>
                </div>
            </g:form>
        </div>
    </body>
</html>
