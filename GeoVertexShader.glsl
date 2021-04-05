#version 410

layout(location = 0) in vec3 modelPos;

void main()
{
    gl_Position = vec4(modelPos, 1.0);
}