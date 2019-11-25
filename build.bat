@echo off

set compiler_flags=-fp:fast -O2 -DASSERTS=0 -DDEVELOPMENT=0 -Gm- -MT -EHa- -Oi -GR- -FC -WX -W4 -nologo -wd4100 -wd4189 -wd4201
set linker_flags=-incremental:no -opt:ref

pushd ..\build
del *.pdb > NUL 2> NUL
echo WAITING FOR PDB > lock.tmp

cl %compiler_flags% ..\code\win32_platform.c /link %linker_flags% Ws2_32.lib user32.lib gdi32.lib winmm.lib
REM Ws2_32.lib user32.lib gdi32.lib winmm.lib

del lock.tmp
del *.obj
popd


REM Compiler Flags:

REM Zi  : debug info (Z7 older debug format for complex builds)
REM Zo  : More debug info for optimized builds
REM FC  : Full path on errors
REM Oi  : Always do intrinsics with you can
REM Od  : No optimizations
REM O2  : Full optimizations
REM MT  : Use the c static lib instead of searching for dll at run-time
REM MTd : Sabe as MT but using the debug version of CRT
REM GR- : Turn off C++ run-time type info
REM Gm- : Turn off incremental build
REM EHa-: Turn off exception handling
REM WX  : Treat warning as errors
REM W4  : Set Warning level to 4 (Wall to all levels)
REM wd  : Ignore warning
REM fp:fast    : Ignores the rules in some cases to optimize fp operations
REM Fmfile.map : Outputs a map file (mapping of the functions on the exr)

REM Linker Options:

REM subsystem:windows,5.1 : Make exe compatible with Windows XP (only works on x86)
REM opt:ref               : Don't put unused things in the exe
REM incremental:no        : Don't need to do incremental builds
REM LD                    : Build a dll
REM PDB:file.pdb          : Change the .pdb's path
