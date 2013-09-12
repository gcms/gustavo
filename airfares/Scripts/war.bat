set CD=%~dp0
call %CD%ambiente.bat

del %WAR_FILE%
call %CD%execute.bat war %WAR_FILE%
