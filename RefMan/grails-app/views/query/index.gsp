<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 10/04/2010
  Time: 13:31:03
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head><title>Simple GSP page</title></head>
<body>
  <g:form name="queryform" action="query" method="POST">
    <g:select id="engine" name="engine" optionKey="id" optionValue="name" from="${engines}"/>
    <g:textField id="query" name="query"/>
    <g:submitButton name="submit" value="Search"/>
    <p>
      <g:textField id="url" name="url"/>
    </p>    
  </g:form>
</body>
</html>