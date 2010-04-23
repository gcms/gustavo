<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 17:06:12
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <g:layoutHead/>
</head>
<body>
<div class="menu">
  <g:if test="${atividade.pai}">
    Atividade pai:
    <g:link controller="atividade" action="view" id="${atividade.pai.id}">${atividade.pai.nome}</g:link> |
  </g:if>
  Workflow
  <g:link controller="workflow" action="view" id="${atividade.workflow.id}">${atividade.workflow.nome}</g:link> |

  Projeto
  <g:link controller="projeto" action="view" id="${atividade.projeto.id}">${atividade.projeto.nome}</g:link>   
  <hr/>
</div>
<div class="body">
  <g:layoutBody/>
</div>
</body>
</html>