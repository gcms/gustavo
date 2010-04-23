<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 12:25:15
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <g:each var="projeto" in="${projetos}">
      <p>
        <g:link action="view" id="${projeto.id}">${projeto.nome}</g:link>
      </p>
    </g:each>
  </body>
</html>