<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 15:57:25
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <h3>Versão ${versao.numero} do produto
      <g:link controller="produto" action="view" id="${versao.produto.id}">${versao.produto.nome}</g:link>.</h3>

    <p>Porcentagem:</p>
    <p>Horas:</p>       
  </body>
</html>