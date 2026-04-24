@ECHO off

glslangValidator -V -S frag ./shaders/frag.glsl -o ./shaders/out/frag.spv -e main

glslangValidator -V -S vert ./shaders/vert.glsl -o ./shaders/out/vert.spv -e main


glslangValidator -V -S frag ./shaders/frag_hud.glsl -o ./shaders/out/frag_hud.spv -e main

glslangValidator -V -S vert ./shaders/vert_hud.glsl -o ./shaders/out/vert_hud.spv -e main



glslangValidator -V -S frag ./shaders/frag_shadow.glsl -o ./shaders/out/frag_shadow.spv -e main

glslangValidator -V -S vert ./shaders/vert_shadow.glsl -o ./shaders/out/vert_shadow.spv -e main