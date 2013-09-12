set CD=%~dp0
call %CD%execute.bat -Dserver.port=8092 test-app
ping 127.0.0.1 -n 5 -w 1000 > NUL
start %PROJETO_HOME%\target\test-reports\html\index.html
