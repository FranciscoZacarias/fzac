@echo off
setlocal

REM Builds raylib 5.5 (desktop, OpenGL 3.3) into Extern\raylib\lib\raylib.lib
REM Requires cl.exe in PATH (run from a Developer Command Prompt or after vcvarsall.bat x64)

where cl >nul 2>nul
if errorlevel 1 (
  echo Error: cl.exe not found. Run vcvarsall.bat x64 first.
  exit /b 1
)

set RAYLIB_DIR=%~dp0
set OUT_DIR=%RAYLIB_DIR%lib

if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"
pushd "%OUT_DIR%"

cl /nologo /c /O2 /Z7 /W0 /MP /DPLATFORM_DESKTOP /DGRAPHICS_API_OPENGL_33 ^
  /I "%RAYLIB_DIR%." /I "%RAYLIB_DIR%external\glfw\include" ^
  "%RAYLIB_DIR%rcore.c" "%RAYLIB_DIR%rshapes.c" "%RAYLIB_DIR%rtextures.c" "%RAYLIB_DIR%rtext.c" ^
  "%RAYLIB_DIR%rmodels.c" "%RAYLIB_DIR%raudio.c" "%RAYLIB_DIR%utils.c" "%RAYLIB_DIR%rglfw.c"
if errorlevel 1 (
  popd
  exit /b 1
)

lib /nologo /OUT:raylib.lib rcore.obj rshapes.obj rtextures.obj rtext.obj rmodels.obj raudio.obj utils.obj rglfw.obj
del *.obj

popd
echo raylib.lib built in %OUT_DIR%
