<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 10/04/2010
  Time: 13:54:12
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <g:each var="${result}" in="${results}">
      <p>
        ${result.title} - ${result.authors.join(', ')} - ${result.publication}<br/>
        ${result.html}
      </p>
      <hr/>
    </g:each>
  </body>
</html>