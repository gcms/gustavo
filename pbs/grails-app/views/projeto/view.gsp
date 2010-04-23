<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 12:25:49
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <h3>${projeto.nome}</h3>
    <g:each var="workflow" in="${projeto.workflows}">
      <g:link controller="workflow" action="view" id="${workflow.id}">${workflow.nome}</g:link>
    </g:each>
  </body>
</html>