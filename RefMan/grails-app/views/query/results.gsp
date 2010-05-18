<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
        <meta name="layout" content="main" />
        <g:set var="entityName" value="${message(code: 'queryResult.label', default: 'QueryResult')}" />
        <title>Query Results</title>
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

            <g:form action="save" method="post" >
                <div class="dialog">
                    <table>
                        <tbody>
                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="engine"><g:message code="queryResult.engine.name.label" default="Engine" /></label>
                                </td>
                                <td valign="top" class="value">
                                    <g:hiddenField name="engine" value="${query?.engine.id}"/>
                                    <g:link action="show" controller="engine" id="${query.engine.id}">${query?.engine.name}</g:link>
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="url"><g:message code="queryResult.url.label" default="URL" /></label>
                                </td>
                                <td valign="top" class="value">
                                  <g:hiddenField name="url" value="${query.url}"/>
                                    <g:link url="${query?.url}">${query?.url}</g:link>                                    
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="name"><g:message code="queryResult.name.label" default="Name" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: queryResultInstance, field: 'name', 'errors')}">
                                  <g:textField name="name" value="${query?.name}" />
                                </td>
                            </tr>

                            <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="description"><g:message code="queryResult.description.label" default="Name" /></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: queryResultInstance, field: 'name', 'errors')}">
                                  <g:textArea name="description" value="${query?.description}" />
                                </td>
                            </tr>


                        </tbody>
                      <tr class="prop">
                                <td valign="top" class="name">
                                    <label for="results"><g:message code="queryResult.results.label" default="Results" /></label>
                                </td>
                                <td valign="top" class="value">
                                    <g:render template="searchResults" var="results" model="[results: query.results]"/>
                                </td>
                            </tr>
                    </table>

                </div>
                <div class="buttons">
                    <span class="button"><g:submitButton name="create" class="save" value="${message(code: 'default.button.save.label', default: 'Save')}" /></span>
                </div>
            </g:form>

        </div>
    </body>
</html>
