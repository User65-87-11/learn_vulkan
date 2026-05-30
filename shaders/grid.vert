// Inputs
uniform mat4 invViewProj;
uniform vec3 cameraPos;

in vec2 uv; // fullscreen quad UV [0..1]

out vec4 FragColor;

float gridLine(float coord, float scale, float thickness)
{
    float v = abs(fract(coord / scale - 0.5) - 0.5) / fwidth(coord / scale);
    return 1.0 - smoothstep(thickness, thickness + 1.0, v);
}

void main()
{
    // Reconstruct world ray
    vec2 ndc = uv * 2.0 - 1.0;

    vec4 nearPoint = invViewProj * vec4(ndc, 0.0, 1.0);
    vec4 farPoint  = invViewProj * vec4(ndc, 1.0, 1.0);

    nearPoint /= nearPoint.w;
    farPoint  /= farPoint.w;

    vec3 rayDir = normalize(farPoint.xyz - nearPoint.xyz);

    // Intersect with plane y = 0
    float t = -cameraPos.y / rayDir.y;

    // Behind camera
    if (t <= 0.0)
        discard;

    vec3 worldPos = cameraPos + rayDir * t;

    // Grid
    float major = gridLine(worldPos.x, 1.0, 1.0);
    major = max(major, gridLine(worldPos.z, 1.0, 1.0));

    float minor = gridLine(worldPos.x, 0.1, 1.0);
    minor = max(minor, gridLine(worldPos.z, 0.1, 1.0));

    vec3 color = vec3(0.0);

    color += vec3(0.15) * minor;
    color += vec3(0.35) * major;

    // Axes
    if (abs(worldPos.x) < 0.01)
        color = vec3(0.0, 0.0, 1.0); // Z axis

    if (abs(worldPos.z) < 0.01)
        color = vec3(1.0, 0.0, 0.0); // X axis

    // Distance fade
    float dist = length(worldPos - cameraPos);
    float fade = exp(-dist * 0.03);

    color *= fade;

    FragColor = vec4(color, 1.0);
}
