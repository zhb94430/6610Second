#version 410

uniform mat4 MVP;
uniform mat4 M;

in vec3 modelPos;
in vec3 modelNor;
in vec2 modelUV;

out vec3 worldPos;
out vec3 worldNor;
out vec2 worldUV;
out vec3 skyboxUV;

void main()
{
    gl_Position = MVP * vec4(modelPos, 1.0);

    worldPos = (M * vec4(modelPos, 1.0)).xyz;
    worldNor = normalize(transpose(inverse(M)) * vec4(modelNor, 1.0)).xyz; 
    worldUV = vec2(modelUV.x, 1.0-modelUV.y); // Flip Texture Coords
    
    skyboxUV = modelPos;
}
