echo "remove shaders/vert.spv shaders/frag.spv"
rm -f shaders/vert.spv shaders/frag.spv 

echo "compile shaders/vert.glsl"
glslc  -fshader-stage=vert -o shaders/vert.spv shaders/vert.glsl 

echo "compile shaders/frag.glsl"
glslc  -fshader-stage=frag -o shaders/frag.spv shaders/frag.glsl 

 