@ECHO off

glslangValidator -Isrc -D__SHADERS__  -V -S frag ./shaders/main.frag -o ./shaders/out/main.frag.spv -e main

glslangValidator -Isrc -D__SHADERS__   -V -S vert ./shaders/main.vert -o ./shaders/out/main.vert.spv -e main


glslangValidator -Isrc -D__SHADERS__  -V -S frag ./shaders/grid.frag -o ./shaders/out/grid.frag.spv -e main

glslangValidator -Isrc -D__SHADERS__   -V -S vert ./shaders/grid.vert -o ./shaders/out/grid.vert.spv -e main


glslangValidator -Isrc -D__SHADERS__  -V -S frag ./shaders/axis.frag -o ./shaders/out/axis.frag.spv -e main

glslangValidator -Isrc -D__SHADERS__   -V -S vert ./shaders/axis.vert -o ./shaders/out/axis.vert.spv -e main


glslangValidator -Isrc -D__SHADERS__  -V -S frag ./shaders/hud.frag -o ./shaders/out/hud.frag.spv -e main

glslangValidator -Isrc -D__SHADERS__   -V -S vert ./shaders/hud.vert -o ./shaders/out/hud.vert.spv -e main
