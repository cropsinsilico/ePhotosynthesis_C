@setlocal
set CONFIGURATION=Release

mkdir conda_build
cd conda_build

:: Call cmake
cmake -GNinja ^
      -D CMAKE_BUILD_TYPE=%CONFIGURATION% ^
      -D CMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% ..
if errorlevel 1 exit 1

:: Using ninja
ninja all
ninja install

:: Using Visual studio
:: if exist czmq.vcxproj (
::     msbuild /v:minimal /p:Configuration=%CONFIGURATION% czmq.vcxproj
:: ) else (
::     msbuild /v:minimal /p:Configuration=%CONFIGURATION% czmq.vcproj
:: )
:: if errorlevel 1 exit 1
:: if exist czmq_selftest.vcxproj (
::     msbuild /v:minimal /p:Configuration=%CONFIGURATION% czmq_selftest.vcxproj
:: ) else (
::     msbuild /v:minimal /p:Configuration=%CONFIGURATION% czmq_selftest.vcproj
:: )

:: Run tests
if errorlevel 1 exit 1
ctest -C "%Configuration%" -V
if errorlevel 1 exit 1

@endlocal
