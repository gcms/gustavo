<html>
<head>
  <title><g:layoutTitle default="Grails"/></title>

  <!-- JQUERY -->
  <link rel="stylesheet" href="${resource(dir: 'css/custom-theme', file: 'jquery-ui-1.8.6.custom.css')}"/>

  <link rel="stylesheet" href="${resource(dir: 'css', file: 'jquery.timeentry.css')}"/>

  <!-- EXT.JS -->

  <link rel="stylesheet" type="text/css" href="${resource(dir: 'js/ext-3.3.0/resources/css', file: 'ext-all.css')}"/>
  <link rel="stylesheet" type="text/css" href="${resource(dir: 'js/ext-3.3.0/resources/css', file: 'xtheme-gray.css')}"/>
  <link rel="stylesheet" type="text/css" href="${resource(dir: 'js/ext-3.3.0/examples/grid', file: 'grid-examples.css')}"/>

  <!-- Common Styles for the examples -->
  <link rel="stylesheet" type="text/css" href="${resource(dir: 'js/ext-3.3.0/examples', file: 'shared/examples.css')}"/>
  <link rel="stylesheet" type="text/css" href="${resource(dir: 'js/ext-3.3.0/examples', file: 'ux/css/RowEditor.css')}"/>

  <!-- DEFAULT -->
  <link rel="stylesheet" href="${resource(dir: 'css', file: 'main.css')}"/>
  <link rel="shortcut icon" href="${resource(dir: 'images/skin', file: 'ambulance.png')}" type="image/x-icon"/>


  <!-- JQUERY -->
  <g:javascript library="jquery-1.4.2.min"/>
  <g:javascript library="jquery-ui-1.8.6.custom.min"/>
  <g:javascript library="jquery.ui.datepicker-pt-BR"/>


  <g:javascript library="jquery.timeentry.min"/>
  <g:javascript library="jquery.timeentry-pt-BR"/>

  <!-- EXT.JS -->
  <script type="text/javascript" src="${resource(dir: 'js/ext-3.3.0', file: 'adapter/ext/ext-base.js')}"></script>
  <script type="text/javascript" src="${resource(dir: 'js/ext-3.3.0', file: 'ext-all-debug.js')}"></script>
  <script type="text/javascript" src="${resource(dir: 'js/ext-3.3.0', file: 'examples/ux/RowEditor.js')}"></script>
  <script type="text/javascript" src="${resource(dir: 'js/ext-3.3.0', file: 'src/locale/ext-lang-pt_BR.js')}"></script>
  %{--<script type="text/javascript" src="${resource(dir: 'js/ext-3.3.0', file: 'adapter/ext/ext-base.js')}"></script>--}%
  %{--<script type="text/javascript" src="${resource(dir: 'js/ext-3.3.0', file: 'adapter/ext/ext-base.js')}"></script>--}%

  %{--<script type="text/javascript" src="../ux/RowEditor.js"></script>--}%
  %{--<script type="text/javascript" src="row-editor.js"></script>--}%

  <!-- DEFAULT -->
  <g:javascript library="application"/>

  <g:layoutHead/>
</head>
<body>
<div id="spinner" class="spinner" style="display:none;">
  <img src="${resource(dir: 'images', file: 'spinner.gif')}" alt="${message(code: 'spinner.alt', default: 'Loading...')}"/>
</div>
<div id="top-main">
  <div id="grailsLogo" class="floating-left">
    <a href="http://intranet-hugo.saude-go.net/">
      <img src="${resource(dir: 'images', file: 'hugo_logo.png')}" alt="Grails" border="0" alt="Home page do HUGO"/>
    </a>
  </div>
  <div class="floating-center">
    <img src="${resource(dir: 'images', file: 'ambulance64x64.png')}" border="0"/>
    <h1>Sistema de Controle de Ambulâncias</h1>
  </div>
  <div class="floating-right">
    <sec:ifLoggedIn>
      <p>
        <span style="font-size: 16pt; font-weight: bold">${operador.nome}</span>
      </p>
      <p>
        Sessão iniciada em <g:formatDate date="${session.login}" formatName="default.dateTime.format"/>
      </p>
    </sec:ifLoggedIn>
  </div>
</div>
<g:layoutBody/>

<div style="clear: both; width: 750px; text-align: left; margin-left: 10px; padding-top: 20px;">
  <hr style="color: #ddd"/>
  Resolução mínima: 1024 x 768
</div>
</body>
</html>
