@echo off

set loc="build"
if not exist %loc% mkdir %loc%

set SDL=C:\lib\SDL2-2.28.3

set includes=/I C:\lib\SDL2-2.28.3\include /I ..\imgui /I ..\imgui\backends
set compiler_flags=/nologo /DEBUG /Zi /W3 /wd4996 /wd4047 /wd4113 /FC /Fe: demo.exe 
set libs=/link %SDL%\lib\x64\SDL2.lib opengl32.lib

pushd %loc%
cl %compiler_flags% %includes% ..\src\main.cpp ..\src\gl32.c ..\imgui\*.cpp ..\imgui\backends\imgui_impl_opengl3.cpp ..\imgui\backends\imgui_impl_sdl2.cpp %libs% 
popd


