@echo off

setlocal
pushd "%~dp0"

set manifest=%~dp0ProviderManifest.man
set resource=%~dp0Provider1.exe

wevtutil.exe im "%manifest%" /rf:"%resource%" /mf:"%resource%" /pf:"%resource%"

popd
endlocal

exit
