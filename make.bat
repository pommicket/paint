@echo off
if "%_VCVARS%" == "" (
	set "_VCVARS=1"
	call vcvarsall x64
)
if "%1" == "" (
	cl /I SDL2\include /nologo /W4 /wd4996 /wd4018 /wd4334 /wd4100 /Zi /Od /DEBUG /DDEBUG=1 main.c shell32.lib SDL2\lib\x64\SDL2.lib SDL2\lib\x64\SDL2main.lib /Fe:paint 
)
if "%1" == "release" (
	cl /I SDL2\include /nologo /W4 /wd4996 /wd4018 /wd4334 /wd4100 /O2 main.c shell32.lib  SDL2\lib\x64\SDL2.lib SDL2\lib\x64\SDL2main.lib /Fe:paint 
)
