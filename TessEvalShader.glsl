#version 410

layout(triangles, equal_spacing, ccw) in; 

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 shadowMVP;
uniform sampler2D b_texDisplacement; 
uniform float displacementScale = 0.4;

in vec3 modelPos_TessEval[];
in vec3 modelNor_TessEval[];
in vec2 modelUV_TessEval[];

out vec3 worldPos_Frag;
out vec4 shadowPos_Frag;
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
	vec3 modelPos_New = interpolateNewAttribute(modelPos_TessEval[0],
			  								    modelPos_TessEval[1],
										 	    modelPos_TessEval[2]);

	vec3 modelNor_New = normalize(interpolateNewAttribute(
											modelNor_TessEval[0],
											modelNor_TessEval[1],
											modelNor_TessEval[2])); 

	worldUV_Frag = interpolateNewAttribute(modelUV_TessEval[0],
										   modelUV_TessEval[1],
										   modelUV_TessEval[2]);

	// Displace the generated vertex
	float displacementValue = texture(b_texDisplacement, worldUV_Frag).x;
	modelPos_New += displacementScale * displacementValue * modelNor_New;

	shadowPos_Frag = shadowMVP * vec4(modelPos_New, 1.0);
	worldPos_Frag = (M * vec4(modelPos_New, 1.0)).xyz;
	worldNor_Frag = normalize(transpose(inverse(M)) * vec4(modelNor_New, 1.0)).xyz;

	gl_Position = MVP * vec4(modelPos_New, 1.0);
}
