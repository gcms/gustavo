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
    <b>Search: ${result.queryResult.engine.name} - ${result.queryResult.name}</b><br/>
    ${result.html}
  </div>
</p>