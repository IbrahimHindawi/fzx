@echo off

mkdir build
pushd build

cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:\devel\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Debug" -G "Visual Studio 17 2022"
rem cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:\devel\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
powershell -Command ..\scripts\clang-build.ps1 -export-jsondb
rem python ..\scripts\finalize.py

popd build
