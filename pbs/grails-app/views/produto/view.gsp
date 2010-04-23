<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 15:59:23
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <h3>${produto.nome}</h3>

      <hr/>
    <g:each var="versao" in="${produto.versoes}">
      <h4>
        Versão <g:link controller="versao" action="view" id="${versao.id}">${versao.numero}</g:link>
      </h4>
        <p>
          Atividade:
          <g:link controller="atividade" action="view" id="${versao.atividade.id}">${versao.atividade.nome}</g:link>
        </p>
        <p>Porcentagem:</p>
        <p>Horas:</p>
      <hr/>
    </g:each>
  </body>
</html>