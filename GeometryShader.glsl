#version 410

layout (triangles) in;
layout (line_strip, max_vertices=4) out;

uniform mat4 VP;

// Compliance for the pipeline
in vec3 worldPos_Frag[];
in vec4 shadowPos_Frag[];
in vec3 worldNor_Frag[];
in vec2 worldUV_Frag[];

void main()
{

	float offset = 0.01;

	// gl_Position = VP * vec4(worldPos_Frag[0] + offset*worldNor_Frag[0], 1.0);
	// gl_Position = VP * vec4(worldPos_Frag[0], 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(offset*worldNor_Frag[0], 0.0);
	EmitVertex();
	// gl_Position = VP * vec4(worldPos_Frag[1] + offset*worldNor_Frag[1], 1.0);
	// gl_Position = VP * vec4(worldPos_Frag[1], 1.0);
	gl_Position = gl_in[1].gl_Position + vec4(offset*worldNor_Frag[1], 0.0);
	EmitVertex();
	// gl_Position = VP * vec4(worldPos_Frag[2] + offset*worldNor_Frag[2], 1.0);
	// gl_Position = VP * vec4(worldPos_Frag[2], 1.0);
	gl_Position = gl_in[2].gl_Position + vec4(offset*worldNor_Frag[2], 0.0);
	EmitVertex();
	// gl_Position = VP * vec4(worldPos_Frag[0] + offset*worldNor_Frag[0], 1.0);
	// gl_Position = VP * vec4(worldPos_Frag[0], 1.0);
	gl_Position = gl_in[0].gl_Position + vec4(offset*worldNor_Frag[0], 0.0);
	EmitVertex();
	EndPrimitive();
}