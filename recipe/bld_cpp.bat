@setlocal
set CONFIGURATION=Release

mkdir conda_build
cd conda_build

:: Call cmake
cmake -GNinja ^
      -D CMAKE_BUILD_TYPE=%CONFIGURATION% ^
      -D CMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% ^
      -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON ^
      -D WITH_YGGDRASIL:BOOL=ON ..
if errorlevel 1 exit 1

:: Using ninja
ninja all
ninja install

:: Run tests
if errorlevel 1 exit 1
ctest -C "%Configuration%" -V
if errorlevel 1 exit 1

@endlocal
