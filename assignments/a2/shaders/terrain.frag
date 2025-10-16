#version 460 core

out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vTex;

uniform sampler2D uTexture;
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;
uniform float uLightIntensity;

void main()
{
    vec3 color = texture(uTexture, vTex).rgb;
    vec3 norm = normalize(vNormal);

    vec3 lightDir = normalize(uLightPos - vFragPos);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 ambient = 0.2 * color;
    vec3 diffuse = diff * color * uLightColor * uLightIntensity;
    vec3 specular = 0.3 * spec * uLightColor * uLightIntensity;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
