<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 28/04/2010
  Time: 10:50:08
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>All Studies</title></head>
  <body>
    <g:each var="study" in="${studies}">
      <p>
        <g:link action="view" id="${study.id}">${study.title}</g:link> - ${study.authorsString} (${study.citationCount})
      </p>
    </g:each>
  </body>
</html>