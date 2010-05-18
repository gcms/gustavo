<%@ page import="br.ufg.inf.refman.SearchResult" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <g:set var="entityName" value="${message(code: 'searchResult.label', default: 'SearchResult')}"/>
  <title><g:message code="default.show.label" args="[entityName]"/></title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
  <span class="menuButton"><g:link class="list" action="view" controller="query" id="${searchResult.queryResult.id}">Query result</g:link></span>
  %{--<span class="menuButton"><g:link class="create" action="create"><g:message code="default.new.label" args="[entityName]" /></g:link></span>--}%
</div>
<div class="body">
  <h1>Search Result<!--<g:message code="default.show.label" args="[entityName]"/>--></h1>
  <g:if test="${flash.message}">
    <div class="message">${flash.message}</div>
  </g:if>
  <div class="dialog">
    <table>
      <tbody>

      <!--
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.id.label" default="Id"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "id")}</td>
                            
                        </tr>
                      -->

      <tr class="prop">
        <td valign="top" class="name"><g:message code="searchResult.queryResult.label" default="Query Result"/></td>

        <td valign="top" class="value">
          <g:link controller="engine" action="show" id="${searchResult?.queryResult?.engine.id}">
            ${searchResult?.queryResult?.engine.name}
          </g:link>:

          <g:link controller="queryResult" action="show" id="${searchResult?.queryResult?.id}">
            ${searchResult?.queryResult?.name}
          </g:link>
        </td>

      </tr>

      <tr class="prop">
        <td valign="top" class="name"><g:message code="searchResult.title.label" default="Title"/></td>

        <td valign="top" class="value">${fieldValue(bean: searchResult, field: "title")}</td>

      </tr>

      <tr class="prop">
        <td valign="top" class="name"><g:message code="searchResult.url.label" default="URL"/></td>

        <td valign="top" class="value">${fieldValue(bean: searchResult, field: "url")}</td>

      </tr>

      <tr class="prop">
        <td valign="top" class="name"><g:message code="searchResult.authorsString.label" default="Authors"/></td>

        <td valign="top" class="value">${fieldValue(bean: searchResult, field: "authorsString")}</td>

      </tr>
      <!--
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.publicationName.label" default="Publication"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "publicationName")}</td>
                            
                        </tr>
                    
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.publicationYear.label" default="Year"/></td>
                            
                            <td valign="top" class="value">${searchResult.publicationYear}</td>
                            
                        </tr>


                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.html.label" default="Html"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "html")}</td>
                            
                        </tr>
                    
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.study.label" default="Study"/></td>
                            
                            <td valign="top" class="value"><g:link controller="study" action="show" id="${searchResult?.study?.id}">${searchResult?.study?.encodeAsHTML()}</g:link></td>
                            
                        </tr>
                        -->
      <tr class="prop">
        <td valign="top" class="name"><g:message code="searchResult.publication.label" default="Publication"/></td>

        <td valign="top" class="value">${fieldValue(bean: searchResult, field: "publication")}</td>

      </tr>

      <tr class="prop">
        <td valign="top" class="name"><g:message code="searchResult.citationCount.label" default="Citation Count"/></td>

        <td valign="top" class="value">${fieldValue(bean: searchResult, field: "citationCount")}</td>

      </tr>

      <!--
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.description.label" default="Description"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "description")}</td>
                            
                        </tr>
                    
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.authors.label" default="Authors"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "authors")}</td>
                            
                        </tr>
                    
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.links.label" default="Links"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "links")}</td>
                            
                        </tr>
                    
                        <tr class="prop">
                            <td valign="top" class="name"><g:message code="searchResult.normalizedTitle.label" default="Normalized Title"/></td>
                            
                            <td valign="top" class="value">${fieldValue(bean: searchResult, field: "normalizedTitle")}</td>
                            
                        </tr>
                        -->

      </tbody>
    </table>
  </div>
  <br/>
  <div class="dialog">
    <table>
      <tbody>

      <tr class="prop">
        <td valign="top" class="name">Similar studies</td>

        <td valign="top" class="value">
        </td>
      </tr>

      <tr class="prop">
        <td valign="top" class="name"></td>

        <td valign="top" class="value">
          <g:each var="study" in="${studies}">
            <p>
              ${study.title}<br/>
              <a href="#" onclick="show(this, 'html${study.id}');">Show details</a>
              <g:link action="associate" id="${study.id}" params="[searchResult: searchResult.id]">Associate</g:link>
            <div style="display: none;" id="html${study.id}">
              <table>
                <tr class="prop">
                  <td valign="top" class="name"><g:message code="study.title.label" default="Title"/></td>

                  <td valign="top" class="value">${fieldValue(bean: study, field: "title")}</td>

                </tr>

                <tr class="prop">
                  <td valign="top" class="name"><g:message code="study.year.label" default="Year"/></td>

                  <td valign="top" class="value">${study.year}</td>

                </tr>

                <tr class="prop">
                  <td valign="top" class="name"><g:message code="study.description.label" default="Description"/></td>

                  <td valign="top" class="value">${fieldValue(bean: study, field: "description")}</td>

                </tr>

                <tr class="prop">
                  <td valign="top" class="name"><g:message code="study.authorsString.label" default="Authors"/></td>

                  <td valign="top" class="value">${fieldValue(bean: study, field: "authorsString")}</td>

                </tr>

                <tr class="prop">
                  <td valign="top" class="name"><g:message code="study.citationCount.label" default="Citation Count"/></td>

                  <td valign="top" class="value">${fieldValue(bean: study, field: "citationCount")}</td>

                </tr>

                <tr class="prop">
                    <td valign="top" class="name"><g:message code="study.results.label" default="Results" /></td>

                    <td valign="top" style="text-align: left;" class="value">
                        <ul>
                        <g:each in="${study.results}" var="r">
                            <li><g:link controller="searchResult" action="show" id="${r.id}">${r?.encodeAsHTML()}</g:link></li>
                        </g:each>
                        </ul>
                    </td>

                </tr>

              </table>
            </div>
            </p>
          </g:each>
        </td>
      </tr>
      </tbody>
    </table>
  </div>
  <div class="buttons">
    <g:form>
      <g:hiddenField name="id" value="${searchResult?.id}" />
      <span class="button"><g:actionSubmit class="save" action="newStudy" value="${message(code: 'default.button.new.label', default: 'Create study')}" /></span>
    </g:form>

    %{--<g:link class="save" action="newStudy" id="${searchResult.id}">Create study</g:link>--}%
  </div>
</div>
</body>
</html>
