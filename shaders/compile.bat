@ECHO off

glslangValidator -Isrc -D__SHADERS__  -V -S frag ./shaders/main.frag -o ./shaders/out/main.frag.spv -e main

glslangValidator -Isrc -D__SHADERS__   -V -S vert ./shaders/main.vert -o ./shaders/out/main.vert.spv -e main
