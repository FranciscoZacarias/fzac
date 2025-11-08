@echo off
setlocal

set compiler=cl
set entry=..\tests\tests.c
set build_dir=build

REM Include directory: src
set cl_default_flags=/Isrc
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
set external_include=/I"..\src\fz_std" /I"..\src\fz_std\external"

REM Ignore specific warnings (start with C4201: nonstandard extension used: nameless struct/union)
set cl_ignore_warnings=/wd4201

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
%compiler% %entry% %cl_default_flags% %cl_ignore_warnings% %external_include% /Fe"tests.exe"
popd
goto end

:end
endlocal