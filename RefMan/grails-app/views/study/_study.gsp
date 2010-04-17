<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 12/04/2010
  Time: 11:46:33
  To change this template use File | Settings | File Templates.
--%>

<p>
  ${result.title} - ${result.authorsString} - ${result.publication}<br/>
  <a href="#" onclick="document.getElementById('html${i}').style.display = 'block';">Show details</a>
  <a href="#" onclick="document.getElementById('html${i}').style.display = 'none';">Hide details</a>
  <div style="display: none;" id="html${i}">
    <p>Engine: ${result.queryResult.engine.name}</p>
    <p>Query: <g:link controller="query" action="view" id="${result.queryResult.id}">${result.queryResult.name}</g:link></p>
    <div style="border: solid black 1px;">
      ${result.html}
    </div>
  </div>
</p>