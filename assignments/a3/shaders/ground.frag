#version 460 core

in VS_OUT {
    vec3 worldPos;
    vec3 normalWS;
    vec2 uv;
    vec4 lightClipPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D uAlbedo;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;

uniform vec3  uFogColor;
uniform float uFogStart;
uniform float uFogEnd;
uniform float uFogDensity;
uniform vec3  uViewPos;

uniform sampler2D shadowMap;
uniform vec2 uShadowMapSize;
uniform float uShadowBias;

float ShadowFactor(vec4 lightClipPos, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = lightClipPos.xyz / lightClipPos.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 ||
    projCoords.y < 0.0 || projCoords.y > 1.0 ||
    projCoords.z > 1.0)
    return 0.0;

    float ndotl = max(dot(normalize(normal), normalize(-lightDir)), 0.0);
    float bias = max(uShadowBias * (1.0 - ndotl), uShadowBias * 0.5);

    vec2 texelSize = 1.0 / uShadowMapSize;
    float shadow = 0.0;
    float current = projCoords.z - bias;
    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            float closest = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += current > closest ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main()
{
    vec3 albedo = texture(uAlbedo, fs_in.uv).rgb;

    vec3 N = normalize(fs_in.normalWS);
    vec3 L = normalize(-uLightDir);
    float ndotl = max(dot(N, L), 0.0);

    float shadow = ShadowFactor(fs_in.lightClipPos, N, uLightDir);
    vec3 diffuse = (1.0 - shadow) * ndotl * uLightColor * albedo;
    vec3 ambient = uAmbientColor * albedo;

    vec3 color = diffuse + ambient;

    float dist = length(uViewPos - fs_in.worldPos);
    float fogFactor = clamp((uFogEnd - dist) / max(uFogEnd - uFogStart, 0.0001), 0.0, 1.0);
    color = mix(uFogColor, color, fogFactor);

    FragColor = vec4(color, 1.0);
}