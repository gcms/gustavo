<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 23/04/2010
  Time: 12:53:48
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head><title>Simple GSP page</title></head>
  <body>

    <h3>${atividade.nome}</h3>
    <g:if test="${atividade.versao}">
      <g:set var="versao" value="${atividade.versao}"/>
      <g:set var="produto" value="${versao.produto}"/>
      Gera versao
      <g:link controller="versao" action="view" id="${versao.id}">${versao.numero}</g:link>
      do produto
      <g:link controller="produto" action="view" id="${produto.id}">${produto.nome}</g:link>.
    </g:if>
    <g:else>
      <p>
        <i>Ainda não foi definido um produto para esta atividade.</i>
      </p>
      
      Esta atividade
      <g:link action="gerar" id="${atividade.id}">gera</g:link> um novo produto ou
      <g:link action="atualizar" id="${atividade.id}">atualiza</g:link> um produto existente?      
    </g:else>
  </body>
</html>