#version 330 core
layout (location = 0) in vec2 aPos;

uniform float uTime;

void main() {
    float x = aPos.x;
    float y = aPos.y + 0.1 * sin(10.0 * aPos.x + uTime * 2.0);
    gl_Position = vec4(x, y, 0.0, 1.0);
}