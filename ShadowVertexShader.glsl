#version 410

uniform mat4 MVP;

layout(location = 0) in vec3 modelPos; // layout declared to reuse VAO from main shader

void main()
{
	gl_Position = MVP * vec4(modelPos, 1.0);
}