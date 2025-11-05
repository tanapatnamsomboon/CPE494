#version 460

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTex;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4  aWeights;

out vec2 TexCoord;

uniform mat4 uModel;
uniform mat4 uBones[200];
uniform bool uUseSkinning;
uniform mat4 uLightVP;

void main()
{
    vec4 localPos = vec4(aPos, 1.0);

    if (uUseSkinning)
    {
        mat4 skin = mat4(0.0);
        if (aBoneIDs.x >= 0) skin += aWeights.x * uBones[aBoneIDs.x];
        if (aBoneIDs.y >= 0) skin += aWeights.y * uBones[aBoneIDs.y];
        if (aBoneIDs.z >= 0) skin += aWeights.z * uBones[aBoneIDs.z];
        if (aBoneIDs.w >= 0) skin += aWeights.w * uBones[aBoneIDs.w];
        localPos = skin * localPos;
    }

    gl_Position = uLightVP * uModel * localPos;
    TexCoord = aTex;
}