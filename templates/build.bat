@echo off
setlocal EnableDelayedExpansion

REM fzac project build template. Copy next to your src\ folder and edit the config block.
REM Usage: build [release] [metaprogram or all] [profile] [raylib]
REM   release     : optimized build
REM   metaprogram : only rebuild metaprogram.exe
REM   all         : rebuild metaprogram.exe, then the app
REM   profile     : defines USE_PROFILER
REM   raylib      : force a rebuild of raylib.lib

cls

for /f %%i in ('echo prompt $E ^| cmd') do set "ESC=%%i"
set "GRN=%ESC%[32m"
set "RED=%ESC%[31m"
set "BLD=%ESC%[1m"
set "CYN=%ESC%[36m"
set "RST=%ESC%[0m"

REM ---- Project config ----------------------------------------------------------
set "APP_NAME=app"
set "ROOT=%~dp0"
set "SRC_DIR=%ROOT%src"
set "FZAC_DIR=%SRC_DIR%\fzac"
set "APP_SOURCE=%SRC_DIR%\entry_point.c"
set "METAPROGRAM_SOURCE=%SRC_DIR%\metaprogram\metaprogram_entry_point.c"
set "BUILD_DIR=%ROOT%build"
set "USE_RAYLIB=0"
set "USE_METAPROGRAM=1"
REM ------------------------------------------------------------------------------

set "CC=cl"
set "COMMON=/nologo /FC /W4 /WX /EHsc"
set "INCLUDES=/I"%SRC_DIR%" /I"%FZAC_DIR%" /I"%FZAC_DIR%\modules""
set "RAYLIB_LIB_DIR=%FZAC_DIR%\Extern\raylib\lib"
set "LINK_OPTS=/link /incremental:no"
set "FLAGS=/Od /Zi /DDEBUG"
set "MODE_LABEL=debug"
set "PROFILER_DEF="

set "BUILD_METAPROGRAM=0"
set "BUILD_APP=1"
set "FORCE_RAYLIB=0"

:parse_args
if "%~1"=="" goto :main

if /i "%~1"=="release" (
    set "FLAGS=/O2 /DNDEBUG"
    set "MODE_LABEL=release"
    shift
    goto :parse_args
)

if /i "%~1"=="metaprogram" (
    set "BUILD_METAPROGRAM=1"
    set "BUILD_APP=0"
    shift
    goto :parse_args
)

if /i "%~1"=="all" (
    set "BUILD_METAPROGRAM=1"
    set "BUILD_APP=1"
    shift
    goto :parse_args
)

if /i "%~1"=="profile" (
    set "PROFILER_DEF=/D USE_PROFILER"
    shift
    goto :parse_args
)

if /i "%~1"=="raylib" (
    set "FORCE_RAYLIB=1"
    shift
    goto :parse_args
)

echo Unknown argument: %~1
exit /b 1

:main
where %CC% >nul 2>nul
if errorlevel 1 (
    echo %RED%%BLD%cl.exe not found. Run vcvarsall.bat x64 first.%RST%
    exit /b 1
)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

if %USE_RAYLIB% EQU 1 (
    set "BUILD_RAYLIB=%FORCE_RAYLIB%"
    if not exist "%RAYLIB_LIB_DIR%\raylib.lib" set "BUILD_RAYLIB=1"
    if !BUILD_RAYLIB! EQU 1 (
        echo %BLD%Building raylib.lib...%RST%
        call "%FZAC_DIR%\Extern\raylib\build_raylib.bat" >nul
        if errorlevel 1 (
            echo %RED%%BLD%FAILED [raylib]%RST%
            exit /b 1
        )
        echo %GRN%%BLD%OK [raylib]%RST%
    )
    set "LINK_OPTS=%LINK_OPTS% /LIBPATH:"%RAYLIB_LIB_DIR%""
)

if %USE_METAPROGRAM% EQU 0 (
    set "BUILD_METAPROGRAM=0"
)

if %USE_METAPROGRAM% EQU 1 if %BUILD_APP% EQU 1 if not exist "%BUILD_DIR%\metaprogram.exe" (
    echo %BLD%metaprogram.exe not found, building metaprogram first...%RST%
    set "BUILD_METAPROGRAM=1"
)

if %BUILD_METAPROGRAM% EQU 1 (
    pushd "%BUILD_DIR%" >nul
    "%CC%" %COMMON% %FLAGS% /D METAPROGRAM %PROFILER_DEF% %INCLUDES% "%METAPROGRAM_SOURCE%" /Fe:"metaprogram.exe" %LINK_OPTS%
    set "BUILD_EXIT_CODE=!ERRORLEVEL!"
    popd >nul

    if !BUILD_EXIT_CODE! NEQ 0 (
        echo %RED%%BLD%FAILED [metaprogram] [%MODE_LABEL%]%RST%
        exit /b !BUILD_EXIT_CODE!
    )
    echo %GRN%%BLD%OK [metaprogram] [%MODE_LABEL%]%RST%
)

if %USE_METAPROGRAM% EQU 1 if %BUILD_APP% EQU 1 (
    pushd "%BUILD_DIR%" >nul
    "%BUILD_DIR%\metaprogram.exe"
    set "RUN_EXIT_CODE=!ERRORLEVEL!"
    popd >nul

    if !RUN_EXIT_CODE! NEQ 0 (
        echo %RED%%BLD%FAILED [metaprogram run] [%MODE_LABEL%]%RST%
        exit /b !RUN_EXIT_CODE!
    )
)

if %BUILD_APP% EQU 1 (
    pushd "%BUILD_DIR%" >nul
    "%CC%" %COMMON% %FLAGS% %PROFILER_DEF% %INCLUDES% "%APP_SOURCE%" /Fe:"%APP_NAME%.exe" %LINK_OPTS%
    set "BUILD_EXIT_CODE=!ERRORLEVEL!"
    popd >nul

    if !BUILD_EXIT_CODE! NEQ 0 (
        echo %RED%%BLD%FAILED [%APP_NAME%] [%MODE_LABEL%]%RST%
        exit /b !BUILD_EXIT_CODE!
    )
    echo %GRN%%BLD%OK [%APP_NAME%] [%MODE_LABEL%]%RST%
)

exit /b 0
