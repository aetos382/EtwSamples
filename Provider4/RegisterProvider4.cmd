@echo off

setlocal
pushd "%~dp0"

set manifest=%~dp0ProviderManifest4.man
set resource=%~dp0Provider4.exe

icacls "%resource%" /grant "NT AUTHORITY\Local Service":RX /Q
wevtutil.exe im "%manifest%" /rf:"%resource%" /mf:"%resource%" /pf:"%resource%"

popd
endlocal

exit /b
