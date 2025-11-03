#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform bool hasTexture;
uniform vec3 baseColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec3 color = hasTexture ? texture(texture_diffuse1, TexCoord).rgb : baseColor;
    vec4 texColor = hasTexture ? texture(texture_diffuse1, TexCoord) : vec4(baseColor, 1.0);
    if (texColor.a < 0.1)
        discard;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    // --- lighting ---
    float diff = max(dot(norm, lightDir), 0.0);
    float specTex = texture(texture_specular1, TexCoord).r;
    float specStrength = mix(0.2, 1.0, specTex);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0) * specStrength;

    vec3 ambient  = 0.2 * color;
    vec3 diffuse  = diff * color * uLightColor;
    vec3 specular = 0.3 * spec * uLightColor;

    vec3 lighting = ambient + diffuse + specular;
    FragColor = vec4(lighting, texColor.a);
}
