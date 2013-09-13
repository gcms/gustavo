<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 12/09/13
  Time: 16:08
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
<head>
  <title></title>
</head>
<body>
${itinerary.price}
<g:each var="route" in="${itinerary.routes}">
    <p> ${route.from} - ${route.to} departure: ${route.departure.format('dd/MM/yyyy HH:mm')} arrival: ${route.arrival.format('dd/MM/yyyy HH:mm')}</p>
</g:each>
</body>
</html>