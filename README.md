# learn_vulkan

to build: zig build
to run: .\zig-out\bin\main.exe



added debug.glsl with 8 segment display that can print 0000-9999 number

located in shaders/debug.glsl

use in frag shader:

#extension GL_GOOGLE_include_directive : require

#include "debug.glsl"

...

if (draw_9999(99, gl_FragCoord.xy, fb_size / 2)) {

      texColor = vec4(0.0, 2.0, 0.0, 1.0);
}
