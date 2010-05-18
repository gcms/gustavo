<%@ page import="br.ufg.inf.refman.Study" %>
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <g:set var="entityName" value="${message(code: 'study.label', default: 'Study')}"/>
  <title><g:message code="default.edit.label" args="[entityName]"/></title>
  <script type="text/javascript">
    function removeItem(id) {
      var form = document.forms[0];
      form.action = 'removeItem?item=' + id;
      form.submit();
    }

    function addItem(id) {
      var form = document.forms[0];
      form.action = 'addItem?item=' + id;
      form.submit();
    }
  </script>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${createLink(uri: '/')}">Home</a></span>
  <span class="menuButton"><g:link class="list" action="list">Studies</g:link></span>
  <span class="menuButton"><g:link class="list" controller="query" action="list">Search queries</g:link></span>
</div>
<div class="body">
  <h1><g:message code="default.edit.label" args="[entityName]"/></h1>
  <g:if test="${flash.message}">
    <div class="message">${flash.message}</div>
  </g:if>
  <g:hasErrors bean="${study}">
    <div class="errors">
      <g:renderErrors bean="${study}" as="list"/>
    </div>
  </g:hasErrors>
  <g:form method="post">
    <g:hiddenField name="id" value="${study?.id}"/>

    <div class="dialog">
      <table>
        <tbody>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="title"><g:message code="study.title.label" default="Title"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'title', 'errors')}">
            <g:textField name="title" value="${study?.title}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="year"><g:message code="study.year.label" default="Year"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'year', 'errors')}">
            <g:textField name="year" value="${study.year}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="description"><g:message code="study.description.label" default="Description"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'description', 'errors')}">
            <g:textField name="description" value="${study?.description}"/>
          </td>
        </tr>
        <!--
                            <tr class="prop">
                                <td valign="top" class="name">
                                  <label for="authors"><g:message code="study.authors.label" default="Authors"/></label>
                                </td>
                                <td valign="top" class="value ${hasErrors(bean: study, field: 'authors', 'errors')}">
                                    
                                </td>
                            </tr>
-->
        <tr class="prop">
          <td valign="top" class="name">
            <label for="authorsString"><g:message code="study.authorsString.label" default="Authors"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'authorsString', 'errors')}">
            <g:textField name="authorsString" value="${study?.authorsString}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="citationCount"><g:message code="study.citationCount.label" default="Citation Count"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'citationCount', 'errors')}">
            <g:textField name="citationCount" value="${fieldValue(bean: study, field: 'citationCount')}"/>
          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            Information
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'results', 'errors')}">
            <g:set var="query" value="${'allintitle:' + study.normalizedTitle.replaceAll(' ', '.')}"/>
            <iframe src="http://scholar.google.com/scholar?q=${query}" width="100%" height="320px"></iframe>
          </td>
        </tr>



        <tr class="prop">
          <td valign="top" class="name">
            <label for="results"><g:message code="study.results.label" default="Results"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'results', 'errors')}">
            <ul>
              <g:each in="${study?.results?}" var="r">
                <li>
                  <g:link controller="searchResult" action="show" id="${r.id}">${r?.encodeAsHTML()}</g:link>
                  (<a href="#" onclick="removeItem(${r.id});">-</a>)
                  <br/>
                  <a href="#" onclick="show(this, 'html${r.id}')">Show details</a>
                  <div style="display: none;" id="html${r.id}">
                    <table>
                    ${r.html}
                    </table>
                  </div>
                </li>
              </g:each>
            </ul>

          </td>
        </tr>

        <tr class="prop">
          <td valign="top" class="name">
            <label for="results"><g:message code="study.results.label" default="Similar results"/></label>
          </td>
          <td valign="top" class="value ${hasErrors(bean: study, field: 'results', 'errors')}">
            <ul>
              <g:each in="${similarResults?}" var="r">
                <li>
                  <g:link controller="searchResult" action="show" id="${r.id}">${r?.encodeAsHTML()}</g:link>
                  (<a href="#" onclick="addItem(${r.id});">+</a>)

                  <br/>
                  <a href="#" onclick="show(this, 'html${r.id}')">Show details</a>
                  <div style="display: none;" id="html${r.id}">
                    <table>
                    ${r.html}
                    </table>
                  </div>
                </li>
              </g:each>
            </ul>

          </td>
        </tr>

        </tbody>
      </table>
    </div>

    <div class="buttons">
      <g:set var="submitAction" value="${fields.acao}"/>
      <span class="button"><g:actionSubmit class="save" action="${submitAction}" value="${message(code: 'study.button.update.label', default: 'Save')}"/></span>
      %{--<span class="button"><g:actionSubmit class="delete" action="delete" value="${message(code: 'default.button.delete.label', default: 'Delete')}" onclick="return confirm('${message(code: 'default.button.delete.confirm.message', default: 'Are you sure?')}');"/></span>--}%
    </div>
  </g:form>
</div>
</body>
</html>
