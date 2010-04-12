<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 12/04/2010
  Time: 09:05:56
  To change this template use File | Settings | File Templates.
--%>

<hr/>
<g:each var="${result}" in="${results}" status="i">
  <p>
    ${result.title} - ${result.authorsString} - ${result.publication} (${result.citationCount})<br/>

    <a href="#" onclick="document.getElementById('html${i}').style.display = 'block';">Show details</a>
    <a href="#" onclick="document.getElementById('html${i}').style.display = 'none';">Hide details</a>
    <g:if test="${result.id}">
      <g:link action="newStudy" controller="study" id="${result.id}">Save study</g:link>
      <g:link action="associate" controller="study" id="${result.id}">Associate to an existing study</g:link>
    </g:if>
    <div style="display: none;" id="html${i}">
      ${result.html}
    </div>
  </p>
  <hr/>
</g:each>