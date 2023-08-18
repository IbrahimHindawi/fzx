@echo off
msbuild build\fzx.sln -nologo -m -v:m /property:Configuration=Debug
rem msbuild build\fzx.sln -nologo -m -v:m /property:Configuration=Release
