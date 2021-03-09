#version 410

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

in vec3 worldPos;
in vec3 worldNor;
in vec2 worldUV;
in vec3 skyboxUV;

out vec3 outputColor;

void main()
{
    float lightDistance = length(l.pos - worldPos);
    vec3 lightDirection = normalize(l.pos - worldPos);
    vec3 cameraDirection = normalize(cameraPos - worldPos);
    vec3 reflecDirection = normalize(reflect(-lightDirection, worldNor));
    vec3 camReflecDirection = normalize(reflect(-cameraDirection, worldNor));

    float cosTheta = clamp(dot(worldPos, lightDirection), 0, 1);
    float cosAlpha = clamp(dot(cameraDirection, reflecDirection), 0, 1);

    // outputColor = l.color;
    // outputColor = worldNor;
    
    if (b.sampleMirror == 1)
    {
        // Retina Scaling
        // outputColor = 0.8*texture(b_texAmbient, vec2(gl_FragCoord.x/2/800, 1-gl_FragCoord.y/2/600)).rgb;

        outputColor = 1.0*texture(b_texAmbient, vec2(gl_FragCoord.x/2/800, 1-gl_FragCoord.y/2/600)).rgb;
    }
    else
    {
        outputColor = 
            b.ambient * texture(b_texAmbient, worldUV).rgb + 
            b.diffuse * texture(b_texDiffuse, worldUV).rgb * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
            b.specular * texture(b_texSpecular, worldUV).rgb * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance) +
            b.specularReflection * texture(skyboxTex, camReflecDirection).rgb + 
            skyboxValue * texture(skyboxTex, skyboxUV).rgb;
    }
    // if (skyboxValue != 0)
    // {
    //     outputColor = skyboxValue * texture(skyboxTex, skyboxUV).rgb;
    //     // outputColor = vec3(1.0, 0.0, 0.0);
    // }
    // else
    // {
    //     outputColor = b.ambient * texture(b_texAmbient, worldUV).rgb + 
    //                   b.diffuse * texture(b_texDiffuse, worldUV).rgb * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
    //                   b.specular * texture(b_texSpecular, worldUV).rgb * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance);

    //     // outputColor = b.ambient + 
    //     //               b.diffuse * l.color * l.power * cosTheta / (lightDistance * lightDistance) +
    //     //               b.specular * l.color * l.power * pow(cosAlpha, 5) / (lightDistance * lightDistance);

    //     // outputColor = vec3(1.0, 0.0, 0.0);
    //     // outputColor = l.color;
    // }    
}
