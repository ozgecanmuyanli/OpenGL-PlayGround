# OpenGL Playground
OpenGL Playground repository for tracking my progress in learning OpenGL.

## BUILD PROJECT FOR VS 2019
    rm -rf build_folder
    mkdir build_folder
    cmake -G "Visual Stuido 16" -S . -B build_folder
    cmake --build build_folder

### IMPORT ASSIMP LIB
* compile assimp-5.0.1 according to instructions in BUILD.md file.
* add a new folder into project's dependencies folder and name it as assimp-5.0.1.
* copy assimp-vc142-mtd.dll and assimp-vc142-mtd.lib from compiled assimp to the new assimp-5.0.1 folder.
* copy include folder of assimp to the new assimp-5.0.1 folder.
* copy include folder to the build_folder/dependencies/assimp-5.0.1 path(if there is no assimp-5.0.1 folder here, create it)
* copy assimp-vc142-mtd.dll to the same path with the project's exe.
* open Visual Studio, Solution Explorer->OpenGL-Playground(right click)->properties->linker->input->additional dependencies(edit)->
  add assimp-vc142-mtd.lib path that is project's dependencies folder (..\..\Dependencies\assimp-5.0.1\assimp-vc142-mtd.lib)
* open Visual Studio, Solution Explorer->OpenGL-Playground(right click)->properties->C/C++->General-> additional include directories->
  add include folder path tha is in build_folder. (C:\OpenGL-workingArea\build\Dependencies\assimp-5.0.1\include)