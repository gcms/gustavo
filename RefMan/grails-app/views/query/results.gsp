<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 10/04/2010
  Time: 13:54:12
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <title>Search results</title>
  %{--<link rel="stylesheet" href="${createLinkTo(dir:'css',file:'main.css')}">    --}%
  %{--</link>--}%
</head>
  <body>
    <fieldset title="Search results">
      <g:form action="save" method="POST">
        <g:hiddenField name="engine" value="${engine}"/>
        <p>
          <label for="name">Name:</label>
          <g:textField name="name"/>
        </p>
        <p>
          <label for="description">Description:</label>
          <g:textArea name="description"/>
        </p>
        <g:hiddenField name="url" value="${url}"/>
        <g:submitButton name="save" value="Save"/>
      </g:form>
    </fieldset>
    <g:render template="searchResults" var="results" model="[results: results]"/>
  </body>
</html>