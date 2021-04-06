#version 410

layout (vertices = 3) out; // Triangle
// layout (vertices = 4) out; // Quad

uniform float tessLevel = 1.0;

in vec3 modelPos_TessCtl[];
in vec3 modelNor_TessCtl[];
in vec2 modelUV_TessCtl[];

// in vec3 worldPos[];
// in vec3 worldNor[];
// in vec2 worldUV[];

out vec3 modelPos_TessEval[];
out vec3 modelNor_TessEval[];
out vec2 modelUV_TessEval[];

void main()
{
	modelPos_TessEval[gl_InvocationID] = modelPos_TessCtl[gl_InvocationID];
	modelNor_TessEval[gl_InvocationID] = modelNor_TessCtl[gl_InvocationID];
	modelUV_TessEval[gl_InvocationID] = modelUV_TessCtl[gl_InvocationID];

	// Set the tesslation level of the triangle
	gl_TessLevelOuter[0] = tessLevel;
	gl_TessLevelOuter[1] = tessLevel;
	gl_TessLevelOuter[2] = tessLevel;
	gl_TessLevelInner[0] = tessLevel;
}