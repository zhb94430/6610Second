#version 410

// out float fragmentDepth;

in vec3 worldPos_Frag;
in vec4 shadowPos_Frag;
in vec3 worldNor_Frag;
in vec2 worldUV_Frag;

out vec3 color;

void main()
{
	// fragmentDepth = gl_FragCoord.z;

	color = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);

	// color = vec3(1.0, 0.0, 0.0);
}