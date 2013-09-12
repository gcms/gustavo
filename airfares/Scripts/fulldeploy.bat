set CD=%~dp0
call %CD%ambiente.bat

call %CD%svnup.bat
call %CD%test-app.bat
call %CD%war.bat
call %CD%deploy.bat
