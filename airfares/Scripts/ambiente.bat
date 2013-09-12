set CD=%~dp0

call %CD%..\configuracao.bat

set PROJETO_HOME=%CD%..\%PROJETO_NOME%
set WORK_HOME=%PROJETO_HOME%\..

set GRAILS_HOME=%WORK_HOME%\..\grails-%GRAILS_VERSION%
set PATH=%GRAILS_HOME%\bin;%WORK_HOME%\Scripts;%PATH%

set WAR_FILE=%WORK_HOME%\%PROJETO_NOME%.war

cd %WORK_HOME%
cd %PROJETO_HOME%
