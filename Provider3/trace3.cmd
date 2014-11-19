@echo off

setlocal
pushd "%~dp0"

set name=trace_test3
set guid={824E8551-8E4C-43E3-B16A-9FEC1A7632BA}

if exist "%~dp0%name%*.etl" del "%~dp0%name%*.etl"
if exist "%~dp0%name%*.xml" del "%~dp0%name%*.xml"

logman start trace %name% -p %guid% -o "%~dp0%name%.etl" -ets

Provider3.exe

logman stop "%name%" -ets

popd
endlocal

exit /b
