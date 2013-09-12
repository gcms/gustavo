<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 12/09/13
  Time: 12:22
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <title></title>
</head>
<body>
<h1>${query}</h1>
<g:each var="price" in="${bestPrices}">
    <p>${price.day.format('dd/MM/yyyy')} ${price.currency} ${price.price}</p>
</g:each>
</body>
</html>