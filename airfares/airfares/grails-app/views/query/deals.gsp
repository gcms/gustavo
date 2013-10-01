<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 16/09/13
  Time: 08:59
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <title></title>
</head>
<body>


<table>
    <tr>
        <th>Pesquisa</th>
        <th>Preço hoje</th>
        <th>Preço médio</th>
        <th>Desconto</th>
    </tr>
    <g:each var="deal" in="${deals}">
        <g:set var="query" value="${deal.query}"/>
        <tr>
            <td>
                <g:link action="view" id="${query.id}">${query}</g:link>
            </td>
            <td>
                <g:link action="best" id="${query.id}">${query.currentPrice}</g:link>
            </td>
            <td>
                ${deal.average}
            </td>
            <td>
                ${deal.discount} %
            </td>
        </tr>
    </g:each>

</table>

</body>
</html>