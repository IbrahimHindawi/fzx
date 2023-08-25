# fzx 
C physics programming using SDL2
Compiles to Win64 MacOS Linux with `compile_commands.json` for clangd LSP support.

### Linux & Darwin Run:
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`
* `./cgfx`

### Win64 Run:  
* open `x64 Native Tools Command Prompt for VS 2019`  
* [Install & Integrate VCPKG](https://learn.microsoft.com/en-us/vcpkg/examples/installing-and-using-packages)
* Install dependencies through VCPKG.
* Configure CMakeToolchainFIle in `scripts\build.bat`.
* `scripts\build.bat -b` to build `.sln` & `compile_commands.json`.
* `scripts\build.bat -cr` to compile & run exe.

### Win64 Toolchain:
* [Visual Studio 2022 & MSBuild](https://visualstudio.microsoft.com/downloads/)
* [Clang Power Tools VS Extension](https://marketplace.visualstudio.com/items?itemName=caphyon.ClangPowerTools)
* [vcpkg](https://github.com/Microsoft/vcpkg/)  
* [CMake](https://cmake.org/)

### Dependencies:
* [SDL2](https://www.libsdl.org/)  
* [SDL2_gfx](https://www.libsdl.org/)  

### TODO:
* automate build scripts to become project agnostic
