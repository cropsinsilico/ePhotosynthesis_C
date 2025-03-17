@setlocal
set CONFIGURATION=Release

mkdir conda_build
cd conda_build

rem Call cmake
rem -GNinja ^
rem -D CMAKE_BUILD_TYPE=%CONFIGURATION% ^
rem -D CMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% ^
cmake -G "Ninja" ^
      -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON ^
      -D WITH_YGGDRASIL:BOOL=ON ^
      -D "Python_EXECUTABLE:FILEPATH=%PYTHON%" ..
if errorlevel 1 exit 1

rem Build all, run tests, then install
cmake --build . --config %CONFIGURATION%
cmake --install .

rem Using ninja
rem ninja all
rem ninja install

rem Run tests
if errorlevel 1 exit 1
ctest -C "%Configuration%" -V
if errorlevel 1 exit 1

@endlocal
