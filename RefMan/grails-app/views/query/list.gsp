
<%@ page import="br.ufg.inf.refman.QueryResult" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'queryResult.label', default: 'QueryResult')}" />
        <title><g:message code="default.list.label" args="[entityName]" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
            <span class="menuButton"><g:link class="create" action="create"><g:message code="queryResult.new.label" args="[entityName]" /></g:link></span>
        </div>
        <div class="body">
            <h1><g:message code="queryResult.list.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
            <div class="message">${flash.message}</div>
            </g:if>
            <div class="list">
                <table>
                    <thead>
                        <tr>

                          <!--
                            <g:sortableColumn property="id" title="${message(code: 'queryResult.id.label', default: 'Id')}" />
                          -->

                            <g:sortableColumn property="engine" title="${message(code: 'queryResult.engine.name.label', default: 'Engine')}" />

                            <g:sortableColumn property="name" title="${message(code: 'queryResult.name.label', default: 'Name')}" />

                            <g:sortableColumn property="resultCount" title="${message(code: 'queryResult.resultCount.label', default: 'Results')}" />

                            <g:sortableColumn property="description" title="${message(code: 'queryResult.description.label', default: 'Description')}" />

                            <g:sortableColumn property="date" title="${message(code: 'queryResult.date.label', default: 'Date')}" />
                            <th>Ações</th>

                        </tr>
                    </thead>
                    <tbody>
                    <g:each in="${queries}" status="i" var="query">
                        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

                          <!--
                            <td><g:link action="show" id="${query.id}">${fieldValue(bean: query, field: "id")}</g:link></td>
                            -->

                            <td>${fieldValue(bean: query, field: "engine.name")}</td>

                            <td><g:link action="view" id="${query.id}">${fieldValue(bean: query, field: "name")}</g:link></td>

                            <td>${fieldValue(bean: query, field: "resultCount")}</td>                            

                            <td>${fieldValue(bean: query, field: "description")}</td>

                            <td><g:formatDate date="${query.date}" /></td>
                            <td>
                              <g:link action="delete" id="${query.id}">X</g:link>
                            </td>
                        </tr>
                    </g:each>
                    </tbody>
                </table>
            </div>
            <div class="paginateButtons">
                <g:paginate max="100" total="${queriesCount}" />
            </div>
        </div>
    </body>
</html>
