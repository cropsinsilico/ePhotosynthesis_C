@setlocal
set CONFIGURATION=Release

mkdir conda_build
cd conda_build

cmake -G "Ninja" ^
      -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON ^
      -D WITH_YGGDRASIL:BOOL=ON ^
      -D "Python_EXECUTABLE:FILEPATH=%PYTHON%" ^
      ..
if errorlevel 1 exit 1

rem Build all, run tests, then install
cmake --build . --config %CONFIGURATION%
cmake --install .

rem Run tests
if errorlevel 1 exit 1
ctest -C "%Configuration%" -V
if errorlevel 1 exit 1

@endlocal
