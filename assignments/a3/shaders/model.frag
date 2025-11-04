#version 460 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform bool hasTexture;
uniform vec3 baseColor;

uniform vec3 uLightColor;
uniform vec3 uLightDir;
uniform vec3 uViewPos;

uniform mat4 uLightVP;
uniform sampler2D shadowMap;
uniform float uShadowBias;
uniform vec2 uShadowMapSize;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

float ShadowFactor(vec3 worldPos, vec3 normal, vec3 lightDir)
{
    vec4 lightClip = uLightVP * vec4(worldPos, 1.0);
    vec3 ndc = lightClip.xyz / lightClip.w;
    vec3 uvw = ndc * 0.5 + 0.5;

    if (uvw.x < 0.0 || uvw.x > 1.0 || uvw.y < 0.0 || uvw.y > 1.0)
        return 0.0;

    float bias = max(uShadowBias * (1.0 - dot(normal, lightDir)), uShadowBias * 0.5);

    float shadow = 0.0;
    vec2 texel = 1.0 / uShadowMapSize;
    for (int x = -1; x <= 1; ++x)
    for (int y = -1; y <= 1; ++y)
    {
        float closest = texture(shadowMap, uvw.xy + vec2(x, y) * texel).r;
        float current = uvw.z - bias;
        shadow += current > closest ? 1.0 : 0.0;
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{
    vec3 color = hasTexture ? texture(texture_diffuse1, TexCoord).rgb : baseColor;
    vec4 texColor = hasTexture ? texture(texture_diffuse1, TexCoord) : vec4(baseColor, 1.0);
    if (texColor.a < 0.1)
        discard;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-uLightDir);
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float specTex = texture(texture_specular1, TexCoord).r;
    float specStrength = mix(0.2, 1.0, specTex);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0) * specStrength;

    float shadow = ShadowFactor(FragPos, norm, lightDir);

    vec3 ambient  = 0.2 * color;
    vec3 diffuse  = (1.0 - shadow) * diff * color * uLightColor;
    vec3 specular = (1.0 - shadow) * 0.3 * spec * uLightColor;

    vec3 lighting = ambient + diffuse + specular;
    FragColor = vec4(lighting, texColor.a);
}
