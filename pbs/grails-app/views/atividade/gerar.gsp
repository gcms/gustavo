<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 12:59:57
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>
    <h3>${atividade.nome}</h3>
    <fieldset>
      <legend>Produto</legend>
      <g:form action="salvarProdutoVersao" id="${atividade.id}">
        <p>
          <label for="pai">Pai:</label>
          <g:select name="pai" optionKey="id" optionValue="nome" from="${produtos}" noSelection="[0: '-- Sem pai --']"/>          
        </p>
        <p>
          <label for="nome">Nome:</label>
          <g:textField name="nome" value=""/>
        </p>
        <g:submitButton name="submit" value="Ok"/>
      </g:form>
    </fieldset>

  </body>
</html>