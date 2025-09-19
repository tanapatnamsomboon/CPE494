#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat3 uTransform;

void main() {
    vec3 pos = uTransform * vec3(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);
}