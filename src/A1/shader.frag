#version 330 core
out vec4 FragColor;

uniform float uTime;

void main() {
    float r = 0.5 + 0.5 * sin(uTime + gl_FragCoord.x * 0.01);
    float g = 0.5 + 0.5 * sin(uTime * 1.3 + gl_FragCoord.y * 0.01);
    float b = 0.5 + 0.5 * sin(uTime * 0.7);

    FragColor = vec4(r, g, b, 1.0);
}