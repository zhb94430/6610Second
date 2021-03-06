#version 330

struct Light 
{
    vec3 pos;
    vec3 color;
    float power;
};

// Material
struct Blinn
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    sampler2D texAmbient;
    sampler2D texDiffuse;
    sampler2D texSpecular;
};

uniform vec3 cameraPos;
uniform Light l;
uniform Blinn b;

in vec3 worldPos;
in vec3 worldNor;
in vec2 worldUV;

out vec3 outputColor;

void main()
{
    float lightDistance = length(l.pos - worldPos);
    vec3 lightDirection = normalize(l.pos - worldPos);
    vec3 cameraDirection = normalize(cameraPos - worldPos);
    vec3 reflecDirection = normalize(reflect(-lightDirection, worldNor));

    float cosTheta = clamp(dot(worldPos, lightDirection), 0, 1);
    float cosAlpha = clamp(dot(cameraDirection, reflecDirection), 0, 1);

    // outputColor = l.color;
    // outputColor = worldNor;

    outputColor = b.ambient * texture(b.texAmbient, worldUV).rgb + 
                  b.diffuse * texture(b.texDiffuse, worldUV).rgb * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
                  b.specular * texture(b.texSpecular, worldUV).rgb * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance);
}
