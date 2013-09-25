<%--
  Created by IntelliJ IDEA.
  User: gustavosousa
  Date: 12/09/13
  Time: 12:22
  To change this template use File | Settings | File Templates.
--%>

<%@ page import="grails.converters.JSON" contentType="text/html;charset=UTF-8" %>
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

<script type="text/javascript" src="https://www.google.com/jsapi"></script>
<script type="text/javascript">
    google.load("visualization", "1", {packages:["corechart"]});
    google.setOnLoadCallback(drawChart);
    function drawChart() {
        var JSONObject = ${dataTable as JSON};
        var data = new google.visualization.DataTable(JSONObject);

        var options = {
            title: '${query.encodeAsJavaScript()}',
            hAxis: {title: 'Day',  titleTextStyle: {color: '#333'}},
            vAxis: {minValue: 0}
        };

        var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
        chart.draw(data, options);
    }
</script>


<h2>Últimos preços</h2>
<g:each var="price" in="${bestPrices}">
    <p>${price.day.format('dd/MM/yyyy')} ${price.currency} ${price.price}</p>
</g:each>

<div id="chart_div"></div>

<g:form action="reload" method="POST">
    <g:hiddenField name="redirectAction" value="view"/>
    <g:hiddenField name="id" value="${query.id}"/>
    <g:submitButton name="reload" value="Recarregar"/>
</g:form>
</body>
</html>