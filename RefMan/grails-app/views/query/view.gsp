
<%@ page import="br.ufg.inf.refman.QueryResult" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'queryResult.label', default: 'QueryResult')}" />
        <title><g:message code="queryResult.edit.label" args="[entityName]" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
            <span class="menuButton"><g:link class="list" action="list"><g:message code="queryResult.list.label" args="[entityName]" /></g:link></span>
            <span class="menuButton"><g:link class="create" action="create"><g:message code="queryResult.new.label" args="[entityName]" /></g:link></span>
        </div>
        <div class="body">
            <h1><g:message code="queryResult.edit.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
            <div class="message">${flash.message}</div>
            </g:if>
            <g:hasErrors bean="${query}">
            <div class="errors">
                <g:renderErrors bean="${query}" as="list" />
            </div>
            </g:hasErrors>
            <g:form method="post" >
                <g:hiddenField name="id" value="${query?.id}" />
                <g:hiddenField name="version" value="${query?.version}" />
                <div class="dialog">
                    <table>
                        <tbody>


                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="engine.id"><g:message code="queryResult.engine.label" default="Engine" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: query, field: 'engine', 'errors')}">
                                    <g:hiddenField name="engine.id" value="${query.engine.id}"/>
                                    <g:link controller="engine" action="show" id="${query?.engine?.id}">${query?.engine?.name}</g:link>
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="name"><g:message code="queryResult.name.label" default="Name" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: query, field: 'name', 'errors')}">
                                    <g:textField name="name" value="${query?.name}" />
                                </td>
                            </tr>


                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="description"><g:message code="queryResult.description.label" default="Description" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: query, field: 'description', 'errors')}">
                                    <g:textArea name="description" value="${query?.description}" />
                                </td>
                            </tr>


                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="date"><g:message code="queryResult.date.label" default="Date" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: query, field: 'date', 'errors')}">
                                    <g:datePicker name="date" precision="day" value="${query?.date}"  />
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="url"><g:message code="queryResult.url.label" default="URL" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: query, field: 'url', 'errors')}">
                                    <g:hiddenField name="url" value="${query.url}"/>
                                    <g:link url="${query.url}">${query.url}</g:link>
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="results"><g:message code="queryResult.results.label" default="Results" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: query, field: 'results', 'errors')}">


  <g:render template="searchResults" var="results" model="[results: query.results]"/>

                                </td>
                            </tr>
                        </tbody>
                    </table>
                </div>
                <div class="buttons">
                    <span class="button"><g:actionSubmit class="save" action="update" value="${message(code: 'queryResult.button.update.label', default: 'Update')}" /></span>
                    <span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'queryResult.button.delete.label', default: 'Delete')}" onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');" /></span>
                </div>
            </g:form>
        </div>
    </body>
</html>
