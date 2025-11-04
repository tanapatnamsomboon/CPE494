#version 460

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 uColor;
uniform float uIntensity;

void main()
{
    float dist = length(TexCoord - vec2(0.5));
    float glow = smoothstep(0.5, 0.0, dist);
    FragColor = vec4(uColor * uIntensity * glow, glow);
}