#version 460 core

in VS_OUT {
    vec2 uv;
    float h;
} fs_in;

out vec4 FragColor;

void main()
{
    vec2 p = gl_PointCoord * 2.0 - 1.0;
    float mask = smoothstep(1.0, 0.7, dot(p, p));

    float t = fs_in.h * 0.5 + 0.5;
    vec3 deep = vec3(0.05, 0.10, 0.25);
    vec3 mid  = vec3(0.10, 0.60, 0.95);
    vec3 hot  = vec3(1.00, 0.85, 0.30);

    vec3 col = mix(deep, mid, smoothstep(0.1, 0.7, t));
    col = mix(col, hot, smoothstep(0.7, 1.0, t));

    float grid = 0.5 + 0.5 * sin((fs_in.uv.x + fs_in.uv.y) * 40.0 + t * 6.28318);
    col *= mix(0.85, 1.15, grid);

    FragColor = vec4(col, mask);
    if (FragColor.a < 0.05) discard;
}