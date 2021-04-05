#version 410

layout (triangles) in;
layout (line_strip, max_vertices=4) out;

uniform mat4 MVP;
uniform vec3 cameraPos;

void main()
{
	vec4 worldPos0 = MVP * gl_in[0].gl_Position;
	vec4 worldPos1 = MVP * gl_in[1].gl_Position;
	vec4 worldPos2 = MVP * gl_in[2].gl_Position;

	vec4 cameraDirection0 = vec4(normalize(cameraPos - worldPos0.xyz), 0.0);
	vec4 cameraDirection1 = vec4(normalize(cameraPos - worldPos1.xyz), 0.0);
	vec4 cameraDirection2 = vec4(normalize(cameraPos - worldPos2.xyz), 0.0);

	float offset = 0.01;

	gl_Position = worldPos0 + offset*cameraDirection0;
	// gl_Position = MVP * gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = worldPos1 + offset*cameraDirection1;
	// gl_Position = MVP * gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = worldPos2 + offset*cameraDirection2;
	// gl_Position = MVP * gl_in[2].gl_Position;
	EmitVertex();
	gl_Position = worldPos0 + offset*cameraDirection0;
	// gl_Position = MVP * gl_in[0].gl_Position;
	EmitVertex();
	EndPrimitive();
}