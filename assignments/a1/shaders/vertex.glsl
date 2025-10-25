#version 460 core

layout(location = 0) in vec2 aPos;
uniform float uTime;

out VS_OUT {
    vec2 uv;
    float h;
} vs_out;

void main()
{
    float freq = 6.0;
    float speed = 0.8;

    vec2 uv = aPos * 0.5 + 0.5;

    float w1 = sin(aPos.x * freq + uTime * speed);
    float w2 = sin(aPos.y * freq * 1.2 - uTime * (speed * 1.3));
    float w3 = sin((aPos.x + aPos.y) * freq * 0.7 + uTime * (speed * 0.6));
    float h = (w1 + w2 + w3) / 3.0; // [-1, 1]

    vs_out.uv = uv;
    vs_out.h = h;

    gl_Position = vec4(aPos, 0.0, 1.0);

    gl_PointSize = 2.0 + 1.5 * (h * 0.5 + 0.5); // ~[2, 3.5]
}