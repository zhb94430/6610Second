#version 330

uniform mat4 MVP;
uniform mat4 M;

in vec3 modelPos;
in vec3 modelNor;

out vec3 worldPos;
out vec3 worldNor;

void main()
{
    gl_Position = MVP * vec4(modelPos, 1.0);

    worldPos = (M * vec4(modelPos, 1.0)).xyz;
    worldNor = normalize(transpose(inverse(M)) * vec4(modelNor, 1.0)).xyz; 
}
