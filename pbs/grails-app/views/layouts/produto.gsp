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
  <g:if test="${produto.pai}">
    Produto pai:
    <g:link controller="produto" action="view" id="${produto.pai.id}">${produto.pai.nome}</g:link> |
  </g:if>

  Projeto
  <g:link controller="projeto" action="view" id="${produto.projeto.id}">${produto.projeto.nome}</g:link>

  <hr/>
</div>
<div class="body">
  <g:layoutBody/>
</div>
</body>
</html>