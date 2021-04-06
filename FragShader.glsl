#version 410

struct Light 
{
    vec3 pos;
    vec3 dir;
    vec3 color;
    float power;
    float cutoff;
};

// Material
struct Blinn
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 specularReflection;
    int sampleMirror; // Switch for sampling mirrored texture
};

// Scene Related
uniform vec3 cameraPos;
uniform Light l;
uniform Blinn b;

uniform sampler2D b_texAmbient;
uniform sampler2D b_texDiffuse;
uniform sampler2D b_texSpecular;
uniform sampler2D b_texNormal;
uniform sampler2D shadowMap;

in vec3 worldPos_Frag;
in vec4 shadowPos_Frag;
in vec3 worldNor_Frag;
in vec2 worldUV_Frag;

out vec3 outputColor;

void main()
{
    // if normal map exists, use normal map value, else use supplied worldNormal
    ivec2 normalMapSize = textureSize(b_texNormal, 0);
    vec3 worldNorProcessed = worldNor_Frag;

    if (normalMapSize.x > 1 && normalMapSize.y > 1)
    {
        worldNorProcessed = texture(b_texNormal, worldUV_Frag).xyz;
    }

    float lightDistance = length(l.pos - worldPos_Frag);
    vec3 lightDirection = normalize(l.pos - worldPos_Frag);
    vec3 cameraDirection = normalize(cameraPos - worldPos_Frag);
    vec3 reflecDirection = normalize(reflect(-lightDirection, worldNorProcessed));
    vec3 camReflecDirection = normalize(reflect(-cameraDirection, worldNorProcessed));

    float cosTheta = clamp(dot(worldNorProcessed, lightDirection), 0, 1);
    vec3 halfVector = normalize(cameraDirection + lightDirection);
    float cosAlpha = clamp(dot(worldNorProcessed, halfVector), 0, 1);

    float shadowThreshold = 1.0;
    float shadowBias = 0.004;
    // float shadowBias = clamp(0.005*tan(acos(cosTheta)), 0.001, 0.01);

    if (texture(shadowMap, shadowPos_Frag.xy/shadowPos_Frag.w).z < (shadowPos_Frag.z/shadowPos_Frag.w - shadowBias))
    {
        // if (dot(lightDirection, worldNorProcessed) > 0)
        // {
            shadowThreshold = 0.01;    
        // }
    }
    
    if (b.sampleMirror == 1)
    {
        // Retina Scaling
        // outputColor = 0.8*texture(b_texAmbient, vec2(gl_FragCoord.x/2/800, 1-gl_FragCoord.y/2/600)).rgb;

        outputColor = 1.0*texture(b_texAmbient, vec2(gl_FragCoord.x/2/800, 1-gl_FragCoord.y/2/600)).rgb;
    }
    else
    {
        // Check for spot light 
        float spotLightTheta = dot(lightDirection, normalize(-l.dir));

        if (spotLightTheta > l.cutoff)
        {
            outputColor = 
            b.ambient * texture(b_texAmbient, worldUV_Frag).rgb + 
            b.diffuse * shadowThreshold * texture(b_texDiffuse, worldUV_Frag).rgb * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
            b.specular * shadowThreshold * texture(b_texSpecular, worldUV_Frag).rgb * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance);
        }
        else
        {
            outputColor = b.ambient * texture(b_texAmbient, worldUV_Frag).rgb;
        }
    }
}
