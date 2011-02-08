<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
  <meta name="layout" content="main"/>
  <title>AuditLogEvent List</title>
</head>
<body>
<div class="nav">
  <span class="menuButton"><a class="home" href="${resource(dir: '')}">Home</a></span>
</div>
<div class="body">
  <h1>AuditLogEvent List</h1>
  <g:if test="${flash.message}">
    <div class="message">${flash.message}</div>
  </g:if>
  <div class="list">
    <table>
      <thead>
      <tr>

        <g:sortableColumn property="id" title="Id"/>

        <g:sortableColumn property="lastUpdated" title="Last updated"/>

        <g:sortableColumn property="actor" title="Actor"/>

        %{--<g:sortableColumn property="uri" title="Uri" />--}%
        %{----}%
        %{--<g:sortableColumn property="className" title="Class Name" />--}%

        %{--<g:sortableColumn property="persistedObjectId" title="Persisted Object Id" />--}%

        <g:sortableColumn property="eventName" title="Event name"/>

        <g:sortableColumn property="propertyName" title="Property name"/>

        <g:sortableColumn property="oldValue" title="Old value"/>

        <g:sortableColumn property="newValue" title="New value"/>

        <g:sortableColumn property="persistedObjectVersion" title="Persisted Object Version"/>

      </tr>
      </thead>
      <tbody>
      <g:each in="${auditLogEventInstanceList}" status="i" var="auditLogEventInstance">
        <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">

          <td><g:link controller="auditLogEvent" action="show" id="${auditLogEventInstance.id}">${fieldValue(bean: auditLogEventInstance, field: 'id')}</g:link></td>

          <td>${fieldValue(bean: auditLogEventInstance, field: 'lastUpdated')}</td>

          <td>${fieldValue(bean: auditLogEventInstance, field: 'actor')}</td>

          %{--<td>${fieldValue(bean:auditLogEventInstance, field:'uri')}</td>--}%
          %{----}%
          %{--<td>${fieldValue(bean:auditLogEventInstance, field:'className')}</td>--}%
          %{----}%
          %{--<td>${fieldValue(bean:auditLogEventInstance, field:'persistedObjectId')}</td>--}%

          <td>${fieldValue(bean: auditLogEventInstance, field: 'eventName')}</td>

          <td>${fieldValue(bean: auditLogEventInstance, field: 'propertyName')}</td>

          <td>${fieldValue(bean: auditLogEventInstance, field: 'oldValue')}</td>

          <td>${fieldValue(bean: auditLogEventInstance, field: 'newValue')}</td>

          <td>${fieldValue(bean: auditLogEventInstance, field: 'persistedObjectVersion')}</td>

        </tr>
      </g:each>
      </tbody>
    </table>
  </div>
  <div class="paginateButtons">
    <g:paginate total="${auditLogEventInstanceTotal}"/>
  </div>
</div>
</body>
</html>
