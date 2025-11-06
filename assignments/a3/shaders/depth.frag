#version 460 core

in vec2 TexCoord;

uniform bool uAlphaCutout;
uniform sampler2D texture_diffuse1;

void main()
{
    if (uAlphaCutout)
    {
        float alpha = texture(texture_diffuse1, TexCoord).a;
        if (alpha < 0.3)
            discard;
    }
}