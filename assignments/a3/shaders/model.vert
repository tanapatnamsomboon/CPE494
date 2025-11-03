#version 460

layout(location = 0) in vec3  aPos;
layout(location = 1) in vec3  aNormal;
layout(location = 2) in vec2  aTex;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4  aWeights;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uBones[200];
uniform bool uUseSkinning;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    vec4 localPos = vec4(aPos, 1.0);
    vec3 localNrm = aNormal;

    mat4 skin = mat4(0.0);
    int valid = 0;
    if (uUseSkinning)
    {
        if (aBoneIDs.x >= 0) { skin += aWeights.x * uBones[aBoneIDs.x]; valid++; }
        if (aBoneIDs.y >= 0) { skin += aWeights.y * uBones[aBoneIDs.y]; valid++; }
        if (aBoneIDs.z >= 0) { skin += aWeights.z * uBones[aBoneIDs.z]; valid++; }
        if (aBoneIDs.w >= 0) { skin += aWeights.w * uBones[aBoneIDs.w]; valid++; }
    }

    vec4 skinnedPos = (!uUseSkinning || valid == 0) ? localPos : (skin * localPos);
    mat3 skin3      = (!uUseSkinning || valid == 0) ? mat3(1.0) : mat3(skin);

    vec4 worldPos = uModel * skinnedPos;
    FragPos = worldPos.xyz;

    mat3 normalMatrix = mat3(transpose(inverse(mat3(uModel) * skin3)));
    Normal = normalize(normalMatrix * localNrm);

    TexCoord = aTex;
    gl_Position = uProjection * uView * worldPos;
}