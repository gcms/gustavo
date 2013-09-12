set CD=%~dp0
call %CD%ambiente.bat

set SRC_FILE=%WAR_FILE%
set DST_DIR=%CATALINA_HOME%\webapps\%PROJETO_NOME%
set DST_FILE=%CATALINA_HOME%\webapps\%PROJETO_NOME%.war

@echo Stopping %SERVICE_NAME%...
net stop %SERVICE_NAME%

@echo Copying file...
del %DST_FILE%
del /S /Q /F %DST_DIR%
copy %SRC_FILE% %DST_FILE%

@echo Starting %SERVICE_NAME%...
net start %SERVICE_NAME%
