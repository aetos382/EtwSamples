@echo off

setlocal
pushd "%~dp0"

set manifest=%~dp0ProviderManifest3.man

wevtutil.exe um "%manifest%"

popd
endlocal

exit /b
