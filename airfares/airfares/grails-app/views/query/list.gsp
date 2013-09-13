<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 12/09/13
  Time: 11:54
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <title></title>
</head>
<body>
<g:each var="query" in="${queries}">
    <p>
        <g:link action="view" id="${query.id}">${query}</g:link>
        <g:link action="best" id="${query.id}">${query.currentPrice}</g:link>
    </p>
</g:each>
</body>
</html>