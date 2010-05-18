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
    <span class="${result.study ? 'selected' : 'normal'}">
      ${result.title}<!-- - ${result.authorsString} - ${result.publication}  --> (${result.citationCount})<br/>
    </span>

    <a href="#" onclick="show(this, 'html${i}');">Show details</a>
    %{--<a href="#" onclick="document.getElementById('html${i}').style.display = 'none';">Hide details</a>--}%
    <g:if test="${result.id}">
      <g:if test="${result.study}">
        <g:link action="view" controller="study" id="${result.study.id}">Show study</g:link>
      </g:if>
      <g:else>
        <g:link action="analyse" controller="study" id="${result.id}">Analyse result</g:link>
        %{--<g:link action="associate" controller="study" id="${result.id}">Associate to an existing study</g:link>--}%
      </g:else>      
    </g:if>
    <div style="display: none;" id="html${i}">
      <table>
      ${result.html}
      </table>
    </div>
  </p>
  <hr/>
</g:each>