
<%@ page import="br.gov.go.saude.hugo.ambulancia.Operador" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <title><g:message code="operador.list" default="Operador List" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLinkTo(dir: '')}"><g:message code="home" default="Home" /></a></span>
            <span class="menuButton"><g:link class="create" action="create"><g:message code="operador.new" default="New Operador" /></g:link></span>
        </div>
        <div class="body">
            <h1><g:message code="operador.list" default="Operador List" /></h1>
            <g:if test="${flash.message}">
            <div class="message"><g:message code="${flash.message}" args="${flash.args}" default="${flash.defaultMessage}" /></div>
            </g:if>
            <div class="list">
                <table>
                    <thead>
                        <tr>
                        
                   	    <g:sortableColumn property="id" title="Id" titleKey="operador.id" />
                        
                   	    <g:sortableColumn property="nome" title="Nome" titleKey="operador.nome" />
                        
                   	    <g:sortableColumn property="usuario" title="Usuario" titleKey="operador.usuario" />
                        
                   	    <g:sortableColumn property="senha" title="Senha" titleKey="operador.senha" />
                        
                   	    <g:sortableColumn property="accountExpired" title="Account Expired" titleKey="operador.accountExpired" />
                        
                   	    <g:sortableColumn property="accountLocked" title="Account Locked" titleKey="operador.accountLocked" />
                        
                        </tr>
                    </thead>
                    <tbody>
                    <g:each in="${operadorInstanceList}" status="i" var="operadorInstance">
                        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">
                        
                            <td><g:link action="show" id="${operadorInstance.id}">${fieldValue(bean: operadorInstance, field: "id")}</g:link></td>
                        
                            <td>${fieldValue(bean: operadorInstance, field: "nome")}</td>
                        
                            <td>${fieldValue(bean: operadorInstance, field: "usuario")}</td>
                        
                            <td>${fieldValue(bean: operadorInstance, field: "senha")}</td>
                        
                            <td><g:formatBoolean boolean="${operadorInstance.accountExpired}" /></td>
                        
                            <td><g:formatBoolean boolean="${operadorInstance.accountLocked}" /></td>
                        
                        </tr>
                    </g:each>
                    </tbody>
                </table>
            </div>
            <div class="paginateButtons">
                <g:paginate total="${operadorInstanceTotal}" />
            </div>
        </div>
    </body>
</html>
