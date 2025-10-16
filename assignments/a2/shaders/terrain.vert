#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel = mat4(1.0);
uniform float uHeightScale;

out vec3 vFragPos;
out vec3 vNormal;
out vec2 vTex;

void main()
{
    vec3 scaledPos = vec3(aPos.x, aPos.y * uHeightScale / 15.0, aPos.z);
    vec4 worldPos = uModel * vec4(scaledPos, 1.0);

    vFragPos = worldPos.xyz;
    vNormal  = normalize(mat3(transpose(inverse(uModel))) * aNormal);
    vTex     = aTex;

    gl_Position = uProjection * uView * worldPos;
}
