<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 12/04/2010
  Time: 09:00:40
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <g:each var="${query}" in="${results}">
      <p>
        ${query.engine.name}
        <g:link action="view" id="${query.id}">${query.name}</g:link>
        (<g:remoteLink action="delete" method="POST" id="${query.id}">X</g:remoteLink>)
      </p>
    </g:each>
  </body>
</html>