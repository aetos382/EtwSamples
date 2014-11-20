@echo off

setlocal
pushd "%~dp0"

set name=trace_test4
set guid={6FD882EA-3844-4527-A251-E6C71E064C37}

if exist "%~dp0%name%*.etl" del "%~dp0%name%*.etl"
if exist "%~dp0%name%*.xml" del "%~dp0%name%*.xml"

logman start trace %name% -p %guid% -o "%~dp0%name%.etl" -ets

Provider4.exe

logman stop "%name%" -ets

popd
endlocal

exit /b
