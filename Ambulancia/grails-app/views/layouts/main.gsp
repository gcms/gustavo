<html>
    <head>
        <title><g:layoutTitle default="Grails" /></title>
        <link rel="stylesheet" href="${resource(dir:'css',file:'main.css')}" />
        <link rel="shortcut icon" href="${resource(dir:'images',file:'favicon.ico')}" type="image/x-icon" />

        <link rel="stylesheet" href="${resource(dir: 'css/custom-theme', file: 'jquery-ui-1.8.6.custom.css')}" />

        <link rel="stylesheet" href="${resource(dir: 'css', file: 'jquery.timeentry.css')}" />           

        <g:javascript library="application" />
        
        <g:javascript library="jquery-1.4.2.min" />
        <g:javascript library="jquery-ui-1.8.6.custom.min" />
        <g:javascript library="jquery.ui.datepicker-pt-BR" />

        <g:javascript library="jquery.timeentry.min" />
        <g:javascript library="jquery.timeentry-pt-BR" />

        <g:layoutHead />

    </head>
    <body>
        <div id="spinner" class="spinner" style="display:none;">
            <img src="${resource(dir:'images',file:'spinner.gif')}" alt="${message(code:'spinner.alt',default:'Loading...')}" />
        </div>
        <div id="grailsLogo"><a href="http://grails.org"><img src="${resource(dir:'images',file:'grails_logo.png')}" alt="Grails" border="0" /></a></div>
        <g:layoutBody />
    </body>
</html>