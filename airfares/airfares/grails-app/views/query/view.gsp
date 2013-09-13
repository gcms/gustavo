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
<g:set var="outbound" value="${query.flightQuery.routes.first()}"/>
<g:set var="inbound" value="${query.flightQuery.routes.last()}"/>
<p>
    Saindo de ${outbound.from} para ${outbound.to} entre ${outbound.departureInterval.start.format('dd/MM/yyyy HH:mm')} e ${outbound.departureInterval.end.format('dd/MM/yyyy HH:mm')}
</p>
<p>
    Chegando entre ${inbound.departureInterval.start.format('dd/MM/yyyy HH:mm')} e ${inbound.departureInterval.end.format('dd/MM/yyyy HH:mm')}
</p>


<h2>Últimos preços</h2>
<g:each var="price" in="${bestPrices}">
    <p>${price.day.format('dd/MM/yyyy')} ${price.currency} ${price.price}</p>
</g:each>
</body>
</html>