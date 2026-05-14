# learn_vulkan

in gltf_loader branch there is debug.glsl shader include file .

It draw 7 segment display-like number in range of 0000-9999.



located in shaders/debug.glsl

bool draw_9999(uint val, vec2 v, vec2 o);

use in frag shader:

#extension GL_GOOGLE_include_directive : require

#include "debug.glsl"

...

if (draw_9999(99, gl_FragCoord.xy, fb_size / 2)) {

  texColor = vec4(0.0, 2.0, 0.0, 1.0);
}
