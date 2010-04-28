<%--
  Created by IntelliJ IDEA.
  User: gustavo
  Date: 12/04/2010
  Time: 10:01:34
  To change this template use File | Settings | File Templates.
--%>

<%@ page contentType="text/html;charset=UTF-8" %>
<html>
  <head>
    <title>Simple GSP page</title>
    <script type="text/javascript">
      function removeItem(id) {
        var form = document.forms['study-form'];
        form.action = 'removeItem?item=' + id;
        form.submit();
      }

      function addItem(id) {
        var form = document.forms['study-form'];
        form.action = 'addItem?item=' + id;
        form.submit();
      }
    </script>
  </head>
  <body>
    <fieldset title="New Publication">
      <form id="study-form" action="${fields.action}" method="POST">
        <p>
          <label for="title">Title:</label>
          <g:textField name="title" value="${fields.title}" size="${fields.title.length()}"/>
        </p>
        <p>
          <label for="authorsString">Authors:</label>
          <g:textField name="authorsString" value="${fields.authorsString}"/>
        </p>
        <p>
          <label for="year">Year:</label>
          <g:textField name="year" value="${fields.year}"/>
        </p>
        <p>
          <label for="citationCount">Cited by:</label>
          <g:textField name="citationCount" value="${fields.citationCount}"/>
        </p>
        <g:hiddenField name="id" value="${fields.id}"/>
        <g:submitButton name="save" value="Save"/>
      </form>
    </fieldset>

  <!--
  <g:set var="query" value="${'allintitle:' + fields.normalizedTitle.replaceAll(' ', '.')}"/>
  <iframe src="http://scholar.google.com/scholar?q=${query}" width="100%" height="300px"/>
  -->
  <g:set var="query" value="${fields.normalizedTitle.replaceAll(' ', '.')}"/>
  <iframe src="http://www.google.com/search?q=${query}" width="100%" height="300px"/>

    <p><b>Selected</b></p>
    <g:each var="result" in="${selectedResults}">
      <g:render template="study" var="result" model="[result: result, i: result.id]"/>
      <a href="#" onclick="removeItem(${result.id});">Remove</a>
    </g:each>

    <hr/>
    <p><b>Not-selected</b></p>
    <g:each var="result" in="${similarResults}">
      <g:render template="study" var="result" model="[result: result, i: result.id]"/>
      <a href="#" onclick="addItem(${result.id});">Add</a>
    </g:each>
  </body>
</html>