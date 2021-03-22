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

uniform float skyboxValue; // Multiplied when calculating final color
uniform samplerCube skyboxTex;

uniform sampler2D b_texAmbient;
uniform sampler2D b_texDiffuse;
uniform sampler2D b_texSpecular;
uniform sampler2D shadowMap;

in vec3 worldPos;
in vec3 worldNor;
in vec2 worldUV;
in vec3 skyboxUV;
in vec4 shadowPos;

out vec3 outputColor;

void main()
{
    float lightDistance = length(l.pos - worldPos);
    vec3 lightDirection = normalize(l.pos - worldPos);
    vec3 cameraDirection = normalize(cameraPos - worldPos);
    vec3 reflecDirection = normalize(reflect(-lightDirection, worldNor));
    vec3 camReflecDirection = normalize(reflect(-cameraDirection, worldNor));

    float cosTheta = clamp(dot(worldNor, lightDirection), 0, 1);
    // float cosAlpha = clamp(dot(cameraDirection, reflecDirection), 0, 1);
    vec3 halfVector = normalize(cameraDirection + lightDirection);
    float cosAlpha = clamp(dot(worldNor, halfVector), 0, 1);

    float shadowThreshold = 1.0;
    float shadowBias = 0.004;
    // float shadowBias = clamp(0.005*tan(acos(cosTheta)), 0.001, 0.01);

    if (texture(shadowMap, shadowPos.xy/shadowPos.w).z < (shadowPos.z/shadowPos.w - shadowBias))
    {
        // if (dot(lightDirection, worldNor) > 0)
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
            b.ambient * texture(b_texAmbient, worldUV).rgb + 
            b.diffuse * shadowThreshold * texture(b_texDiffuse, worldUV).rgb * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
            b.specular * shadowThreshold * texture(b_texSpecular, worldUV).rgb * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance) +
            b.specularReflection * shadowThreshold * texture(skyboxTex, camReflecDirection).rgb + 
            skyboxValue * texture(skyboxTex, skyboxUV).rgb;
        }
        else
        {
            outputColor = b.ambient * texture(b_texAmbient, worldUV).rgb +
                          skyboxValue * texture(skyboxTex, skyboxUV).rgb;
        }

        // outputColor = 
        //     b.ambient * texture(b_texAmbient, worldUV).rgb + 
        //     b.diffuse * shadowThreshold * texture(b_texDiffuse, worldUV).rgb * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
        //     b.specular * shadowThreshold * texture(b_texSpecular, worldUV).rgb * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance) +
        //     b.specularReflection * shadowThreshold * texture(skyboxTex, camReflecDirection).rgb + 
        //     skyboxValue * texture(skyboxTex, skyboxUV).rgb;

        // outputColor = shadowThreshold;

        // outputColor = vec3(shadowPos.xy/shadowPos.w, 0.0);
        // outputColor = vec3(0.0, 0.0, shadowPos.z/shadowPos.w);
        // outputColor = texture(shadowMap, shadowPos.xy/shadowPos.w).rgb;
    }
}
