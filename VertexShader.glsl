#version 410

layout(location = 0) in vec3 modelPos;
layout(location = 1) in vec3 modelNor;
layout(location = 2) in vec2 modelUV;

out vec3 modelPos_TessCtl;
out vec3 modelNor_TessCtl;
out vec2 modelUV_TessCtl;

// out vec4 shadowPos;


void main()
{

    modelPos_TessCtl = modelPos;
    modelNor_TessCtl = modelNor;
    modelUV_TessCtl = vec2(modelUV.x, 1.0-modelUV.y);

    // worldPos = (M * vec4(modelPos, 1.0)).xyz;
    // worldNor = normalize(transpose(inverse(M)) * vec4(modelNor, 1.0)).xyz; 
    // worldUV = vec2(modelUV.x, 1.0-modelUV.y); // Flip Texture Coords

    // shadowPos = shadowMVP * vec4(modelPos, 1.0);
}