#version 410

// out float fragmentDepth;

out vec3 color;

void main()
{
	// fragmentDepth = gl_FragCoord.z;

	color = vec3(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z);

	// color = vec3(1.0, 0.0, 0.0);
}