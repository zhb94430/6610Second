#version 410

layout(triangles, equal_spacing, ccw) in; 

uniform mat4 VP;
uniform sampler2D b_texDisplacement; 
uniform float displacementScale = 4.0;

in vec3 worldPos_TessEval[];
in vec3 worldNor_TessEval[];
in vec2 worldUV_TessEval[];

out vec3 worldPos_Frag;
out vec3 worldNor_Frag;
out vec2 worldUV_Frag;

// Functions to generate attributes for new points
// from gl_TessCoord
vec3 interpolateNewAttribute(vec3 v0, vec3 v1, vec3 v2)
{
	vec3 weight0 = gl_TessCoord.x * v0;
	vec3 weight1 = gl_TessCoord.y * v1;
	vec3 weight2 = gl_TessCoord.z * v2;

	return weight0+weight1+weight2;
}

vec2 interpolateNewAttribute(vec2 v0, vec2 v1, vec2 v2)
{
	vec2 weight0 = gl_TessCoord.x * v0;
	vec2 weight1 = gl_TessCoord.y * v1;
	vec2 weight2 = gl_TessCoord.z * v2;

	return weight0+weight1+weight2; 
}

void main()
{
	// Calculate new attributes for fragment shader
	worldPos_Frag = interpolateNewAttribute(worldPos_TessEval[0],
											worldPos_TessEval[1],
											worldPos_TessEval[2]);

	worldNor_Frag = normalize(interpolateNewAttribute(
											worldNor_TessEval[0],
											worldNor_TessEval[1],
											worldNor_TessEval[2]));

	worldUV_Frag = interpolateNewAttribute(worldUV_TessEval[0],
										   worldUV_TessEval[1],
										   worldUV_TessEval[2]);

	// Displace the generated vertex
	float displacementValue = texture(b_texDisplacement, worldUV_Frag).x;
	worldPos_Frag += displacementScale * displacementValue * worldNor_Frag;

	gl_Position = VP * vec4(worldPos_Frag, 1.0);
}