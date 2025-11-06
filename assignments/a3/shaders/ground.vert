#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform mat4 uLightVP;

uniform float uGroundUVScale;

out VS_OUT {
    vec3 worldPos;
    vec3 normalWS;
    vec2 uv;
    vec4 lightClipPos;
} vs_out;

void main()
{
    vec4 worldPos4 = uModel * vec4(aPos, 1.0);
    vs_out.worldPos = worldPos4.xyz;

    vs_out.uv = worldPos4.xz * uGroundUVScale;

    mat3 nmat = mat3(uModel);
    vs_out.normalWS = normalize(nmat * aNormal);

    vs_out.lightClipPos = uLightVP * worldPos4;

    gl_Position = uProjection * uView * worldPos4;
}