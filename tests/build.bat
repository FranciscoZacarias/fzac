@echo off
setlocal

REM ================= Compiler Flags =================

REM Suppress MSVC compiler startup banner
set cl_default_flags=/nologo
REM Show full file paths in compiler error messages
set cl_default_flags=%cl_default_flags% /FC
REM Generate complete debug information (PDB files)
set cl_default_flags=%cl_default_flags% /Zi
REM Enable level 4 warnings
set cl_default_flags=%cl_default_flags% /W4
REM Treat all warnings as errors — stops build on any warning
set cl_default_flags=%cl_default_flags% /WX

REM === Configuration ===
set compiler=cl
set entry=..\tests\fzac_tests.c
set build_dir=build

REM === Args ===
set arg=%~1
if "%arg%"==""              set arg=fzac_tests
if /I "%arg%"=="all"        goto build_all
if /I "%arg%"=="hph_fzac"   goto build_hph_fzac
if /I "%arg%"=="fzac_tests" goto build_fzac_tests
goto end

REM === Targets ===

:build_all
call "%~f0" hph_fzac
call "%~f0" fzac_tests
goto end

:build_hph_fzac
echo Running Hephaestus on ../src/os/input/os_input.hph ...
bin\hephaestus.exe --input "../src/os/input/os_input.hph"
goto end

:build_fzac_tests
if not exist "%build_dir%" mkdir "%build_dir%"
pushd "%build_dir%"
echo Compiling %entry%
%compiler% %entry% %cl_default_flags% /Fe"fzac_tests.exe" /link
popd
goto end

:end
endlocal
