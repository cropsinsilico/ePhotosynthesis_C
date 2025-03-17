@setlocal

"%PYTHON%" -m pip install . --no-deps --ignore-installed --no-cache-dir -vvv

rem set CONFIGURATION=Release

rem mkdir conda_build
rem cd conda_build

rem :: Call cmake
rem cmake -GNinja ^
rem       -D CMAKE_BUILD_TYPE=%CONFIGURATION% ^
rem       -D CMAKE_INSTALL_PREFIX=%LIBRARY_PREFIX% ^
rem       -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON ^
rem       -D BUILD_CXX:BOOL=OFF ^
rem       -D BUILD_PYTHON:BOOL=ON ^
rem       -D WITH_YGGDRASIL:BOOL=ON ..
rem if errorlevel 1 exit 1

rem :: Using ninja
rem ninja all
rem ninja install

rem :: Run tests
rem if errorlevel 1 exit 1
rem ctest -C "%Configuration%" -V
rem if errorlevel 1 exit 1

@endlocal
