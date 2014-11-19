@echo off

setlocal
pushd "%~dp0"

set manifest=%~dp0ProviderManifest3.man
set resource=%~dp0Provider3.exe

icacls "%resource%" /grant "NT AUTHORITY\Local Service":RX /Q
wevtutil.exe im "%manifest%" /rf:"%resource%" /mf:"%resource%" /pf:"%resource%"

popd
endlocal

exit /b
