@echo off
setlocal

set compiler=cl
set entry=..\tests\tests.c
set build_dir=build

REM === Directory include ===
REM Include directory: modules
set directory_includes=/I../modules
REM Include directory: external
set directory_includes=%directory_includes% /I../external
REM Include directory: src
set directory_includes=%directory_includes% /I../src

REM === CL Compiler flags ===
REM Suppress logo
set cl_default_flags=%cl_default_flags% /nologo
REM Full path info in diagnostics
set cl_default_flags=%cl_default_flags% /FC
REM Debug information
set cl_default_flags=%cl_default_flags% /Zi
REM Warning level 4
set cl_default_flags=%cl_default_flags% /W4
REM Treat warnings as errors
set cl_default_flags=%cl_default_flags% /WX

REM === Args ===
set arg=%1
if "%arg%"=="" set arg=tests
if /I "%arg%"=="all" goto build_all
if /I "%arg%"=="tests" goto build_tests
if /I "%arg%"=="hph_fz_std" goto build_hph
goto end

REM === Targets ===
:build_all
call "%~f0" hph_fz_std
call "%~f0" tests
goto end

:build_hph
echo Running Hephaestus on ../os/os.hph ...
src\fz_std\bin\hephaestus.exe --input "../os/os.hph"
goto end

:build_tests
if not exist %build_dir% mkdir %build_dir%
pushd %build_dir%
echo Compiling %entry%
%compiler% %entry% %cl_default_flags% %directory_includes% /Fe"tests.exe"
popd
goto end

:end
endlocal