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
    <h1>${projeto.nome}</h1>

    <h4>Workflows</h4>
    <g:each var="workflow" in="${projeto.workflows}">
      <g:link controller="workflow" action="view" id="${workflow.id}">${workflow.nome}</g:link>
    </g:each>
    <hr/>

    <h4>PBS</h4>
  
    <g:cadaItem item="produto" nivel="nivel" items="${projeto.produtos}">
      <g:repeat times="${nivel}">
        &nbsp;&nbsp;&nbsp;&nbsp;
      </g:repeat>
      <g:link controller="produto" action="view" id="${produto.id}">${produto.nome}</g:link><br/>
    </g:cadaItem>
  </body>
</html>