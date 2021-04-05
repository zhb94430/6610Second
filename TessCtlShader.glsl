#version 410

// Tess a quad in this case
layout (vertices = 3) out;

uniform float tessLevel = 10240.0;

in vec3 worldPos[];
in vec3 worldNor[];
in vec2 worldUV[];

out vec3 worldPos_TessEval[];
out vec3 worldNor_TessEval[];
out vec2 worldUV_TessEval[];

void main()
{
	worldPos_TessEval[gl_InvocationID] = worldPos[gl_InvocationID];
	worldNor_TessEval[gl_InvocationID] = worldNor[gl_InvocationID];
	worldUV_TessEval[gl_InvocationID] = worldUV[gl_InvocationID];

	// Set the tesslation level of the quad
	gl_TessLevelOuter[0] = tessLevel;
	gl_TessLevelOuter[1] = tessLevel;
	gl_TessLevelOuter[2] = tessLevel;
	gl_TessLevelInner[0] = tessLevel;
}