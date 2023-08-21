@echo off
msbuild build\fzx.sln -nologo -m -v:m /property:Configuration=Debug /property:VcpkgEnabled=false
rem msbuild build\fzx.sln -nologo -m -v:m /property:Configuration=Release
