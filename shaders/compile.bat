@ECHO off

glslangValidator -V -S frag ./shaders/main.frag.glsl -o ./shaders/out/main.frag.spv -e main

glslangValidator -V -S vert ./shaders/main.vert.glsl -o ./shaders/out/main.vert.spv -e main


glslangValidator -V -S frag ./shaders/hud.frag.glsl -o ./shaders/out/hud.frag.spv -e main

glslangValidator -V -S vert ./shaders/hud.vert.glsl -o ./shaders/out/hud.vert.spv -e main



glslangValidator -V -S frag ./shaders/shadow.frag.glsl -o ./shaders/out/shadow.frag.spv -e main

glslangValidator -V -S vert ./shaders/shadow.vert.glsl -o ./shaders/out/shadow.vert.spv -e main