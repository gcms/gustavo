
<%@ page import="br.ufg.inf.refman.QueryResult" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'queryResult.label', default: 'QueryResult')}" />
        <title><g:message code="default.show.label" args="[entityName]" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
            <span class="menuButton"><g:link class="list" action="list"><g:message code="queryResult.list.label" args="[entityName]" /></g:link></span>
            <span class="menuButton"><g:link class="create" action="create"><g:message code="queryResult.new.label" args="[entityName]" /></g:link></span>
        </div>
        <div class="body">
            <h1>Query Result</h1>
            <g:if test="${flash.message}">
            <div class="message">${flash.message}</div>
            </g:if>
            <div class="dialog">
                <table>
                    <tbody>

                    <!--
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.id.label" default="Id" /></td>

                            <td valign="top" class="value">${fieldValue(bean: query, field: "id")}</td>

                        </tr>
                         -->

                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.engine.label" default="Engine" /></td>

                            <td valign="top" class="value"><g:link controller="engine" action="show" id="${query?.engine?.id}">${query?.engine?.name}</g:link></td>

                        </tr>

                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.name.label" default="Name" /></td>

                            <td valign="top" class="value">${fieldValue(bean: query, field: "name")}</td>

                        </tr>

                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.description.label" default="Description" /></td>

                            <td valign="top" class="value">${fieldValue(bean: query, field: "description")}</td>

                        </tr>

                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.date.label" default="Date" /></td>

                            <td valign="top" class="value"><g:formatDate date="${query?.date}" /></td>

                        </tr>

                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.url.label" default="URL" /></td>                            
                            <td valign="top" class="value">                              
                              <g:link url="${query.url}">${query.url}</g:link>
                            </td>

                        </tr>


                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="queryResult.results.label" default="Results" /></td>
                            
                            <td valign="top" style="text-align: left;" class="value">
                                <ul>
                                <g:each in="${query.results}" var="r">
                                    <li><g:link controller="searchResult" action="show" id="${r.id}">${r?.encodeAsHTML()}</g:link></li>
                                </g:each>
                                </ul>
                            </td>
                            
                        </tr>
                    
                    </tbody>
                </table>
            </div>
            <div class="buttons">
                <g:form>
                    <g:hiddenField name="id" value="${query?.id}" />
                    <span class="button"><g:actionSubmit class="edit" action="edit" value="${message(code: 'default.button.edit.label', default: 'Edit')}" /></span>
                    <span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'default.button.delete.label', default: 'Delete')}" onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');" /></span>
                </g:form>
            </div>
        </div>
    </body>
</html>
