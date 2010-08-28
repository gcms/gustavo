
<%@ page import="br.ufg.inf.refman.SearchResult" %>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'searchResult.label', default: 'SearchResult')}" />
        <title><g:message code="default.list.label" args="[entityName]" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
            <span class="menuButton"><g:link class="list" controller="query" action="list"><g:message code="queryResult.list.label" args="[entityName]" /></g:link></span>
        </div>
        <div class="body">
            <h1><g:message code="searchResult.list.label" args="[entityName]" /></h1>
            <g:if test="${flash.message}">
            <div class="message">${flash.message}</div>
            </g:if>
            <div class="list">
                <table>
                    <thead>
                        <tr>

                          <!--
                            <g:sortableColumn property="id" title="${message(code: 'searchResult.id.label', default: 'Id')}" />
                          -->

                            <g:sortableColumn property="title" title="${message(code: 'searchResult.title.label', default: 'Title')}" />

                            <g:sortableColumn property="citationCount" title="${message(code: 'searchResult.citationCount.label', default: 'Citation Count')}" />

                            <g:sortableColumn property="name" title="${message(code: 'searchResult.authorsString.label', default: 'Authors')}" />
                            %{--<th>Ações</th>--}%

                        </tr>
                    </thead>
                    <tbody>
                    <g:each in="${results}" status="i" var="query">
                        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

                          <!--
                            <td><g:link controller="study" action="analyse" id="${query.id}">${fieldValue(bean: query, field: "id")}</g:link></td>
                            -->

                            <td><g:link controller="study" action="analyse" id="${query.id}">${fieldValue(bean: query, field: "title")}</g:link></td>

                            <td>${fieldValue(bean: query, field: "citationCount")}</td>

                            <td>${fieldValue(bean: query, field: "authorsString")}</td>

                            %{--<td>${fieldValue(bean: query, field: "resultCount")}</td>                            --}%

                            %{--<td>${fieldValue(bean: query, field: "description")}</td>--}%

                            %{--<td><g:formatDate date="${query.date}" /></td>--}%
                            %{--<td>--}%
                              %{--<g:link action="delete" id="${query.id}">X</g:link>--}%
                            %{--</td>--}%
                        </tr>
                    </g:each>
                    </tbody>
                </table>
            </div>
            <div class="paginateButtons">
                <g:paginate max="100" total="${resultsCount}" />
            </div>
        </div>
    </body>
</html>
