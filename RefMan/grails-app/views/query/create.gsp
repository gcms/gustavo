
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'queryResult.label', default: 'QueryResult')}" />
        <title><g:message code="queryResult.create.label" args="[entityName]" /></title>
    </head>
    <body>
        <div class="nav">
            <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
            <span class="menuButton"><g:link class="list" action="list"><g:message code="queryResult.list.label" args="[entityName]" /></g:link></span>
        </div>
        <div class="body">
            <h1>New Query</h1>
            <g:if test="${flash.message}">
            <div class="message">${flash.message}</div>
            </g:if>

            <g:form action="query" method="post" >
                <div class="dialog">
                    <table>
                        <tbody>
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="engine"><g:message code="queryResult.engine.name.label" default="Engine" /></label>
                                </td>
                                <td valign="top" class="value">
                                    <g:select name="engine" from="${br.ufg.inf.refman.Engine.list([sort: 'name'])}" optionKey="id" optionValue="name" value="${query?.engine?.id}"  />
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="query">Query</label>
                                </td>
                                <td valign="top" class="value">
                                    <g:textField name="query" value="" />
                                </td>
                            </tr>


                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="url"><g:message code="queryResult.url.label" default="URL" /></label>
                                </td>
                                <td valign="top" class="value">
                                    <g:textField name="url" value="${query?.url}" />
                                </td>
                            </tr>

                        </tbody>
                    </table>
                </div>
                <div class="buttons">
                    <span class="button"><g:submitButton name="create" class="save" value="${message(code: 'queryResult.button.search.label', default: 'Search')}" /></span>
                </div>
            </g:form>
        </div>
    </body>
</html>
