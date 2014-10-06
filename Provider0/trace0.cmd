@echo off

setlocal
pushd "%~dp0"

if exist "%~dp0%name%*.etl" del "%~dp0%name%*.etl"
if exist "%~dp0%name%*.xml" del "%~dp0%name%*.xml"

set name=trace_test
set guid={FF5D07C6-03BA-481F-B07F-97CB8A51FADF}

logman create trace %name% -p %guid% -o "C:\Users\Kazu\Documents\Visual Studio\Projects\EtwSamples\Debug\%name%.etl" -ets

Provider0.exe

logman stop "%name%" -ets

tracerpt "%~dp0%name%.etl" -o "%~dp0%name%.xml"

popd
endlocal

exit /b
