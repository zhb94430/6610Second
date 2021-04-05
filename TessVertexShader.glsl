#version 410

layout(location = 0) in vec3 modelPos;
layout(location = 1) in vec3 modelNor;
layout(location = 2) in vec2 modelUV;

uniform mat4 M;

out vec3 worldPos;
out vec3 worldNor;
out vec2 worldUV;

void main()
{
    worldPos = (M * vec4(modelPos, 1.0)).xyz;
    worldNor = normalize(transpose(inverse(M)) * vec4(modelNor, 1.0)).xyz; 
    worldUV = vec2(modelUV.x, 1.0-modelUV.y); // Flip Texture Coords
}