set CD=%~dp0
call %CD%ambiente.bat
cd %WORK_HOME%

@echo Updating from svn repository
svn up
