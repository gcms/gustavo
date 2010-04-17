<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 12/04/2010
  Time: 09:07:47
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head>
    <title>Query Result</title>
    <style>
      .normal {}
      .selected { font-weight: bold; }
    </style>
  </head>
  <body>
    <p>Engine: ${query.engine.name}</p>
    <p>Name: ${query.name}</p>
    <p>Date: ${query.date}</p>
    <p>Description:<br/>${query.description}</p>
    <g:render template="searchResults" var="results" model="[results: query.results]"/>
  </body>
</html>