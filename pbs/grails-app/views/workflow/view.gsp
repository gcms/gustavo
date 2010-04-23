<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 12:30:46
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
  Projeto <g:link controller="projeto" action="view" id="${workflow.projeto.id}">${workflow.projeto.nome}</g:link>
  <hr/>
    <h3>${workflow.nome}</h3>
    <g:cadaItem nivel="nivel" item="atividade" items="${workflow.atividades}">
      <g:repeat times="${nivel}">
        &nbsp;&nbsp;&nbsp;&nbsp;
      </g:repeat>
      <g:link controller="atividade" action="view" id="${atividade.id}">${atividade.nome}</g:link><br/>
    </g:cadaItem>
  </body>
</html>