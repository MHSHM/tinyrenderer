REM Build script for tinyrenderer
@echo off

REM Get a list of all the .cpp files
set objs=
for /R %%f in (*.cpp) do call set objs=%%objs%% %%f

REM echo "Files:" %objs%

SET assembly=tinyrenderer
SET compilerFlags=-g
SET includeFlags=-Isrc
SET linkerFlags=-luser32

ECHO "Building %assembly%%..."
clang++ %objs% %compilerFlags% -o bin/%assembly%.exe %linkerFlags%