#version 460 core
in vec2 TexCoord;

uniform bool hasTexture;
uniform sampler2D texture_diffuse1;

void main()
{
    if (hasTexture)
    {
        float alpha = texture(texture_diffuse1, TexCoord).a;
        if (alpha < 0.5)
            discard;
    }
}