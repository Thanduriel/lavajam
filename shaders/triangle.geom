#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) in vec4 vs_Position[];
layout(location = 1) in vec3 vs_Color[];
layout(location = 2) in float vs_Rotation[];
layout(location = 3) in float vs_Size[];

//out vec2 gs_texCoord;
layout(location = 0) out vec3 gs_color;

const float c_fAspect = 16.0 / 9.0;

void main()
{
	vec4 position = vs_Position[0];
	float size = vs_Size[0];
	
	float cosa = cos(vs_Rotation[0]);
	float sina = sin(vs_Rotation[0]);
	mat2 rotation = mat2(cosa, sina, -sina, cosa);
	
	vec2 offset = rotation * vec2(0, -1.3 * size); offset.y *= c_fAspect;
	gl_Position = position + vec4(offset, 0, 0);
	gs_color = vs_Color[0];
	EmitVertex();
	offset = rotation * vec2(size,  size); offset.y *= c_fAspect;
	gl_Position = position + vec4(offset, 0, 0);
	gs_color = vs_Color[0];
	EmitVertex();
	offset = rotation * vec2(-size, size); offset.y *= c_fAspect;
	gl_Position = position + vec4(offset, 0, 0);
	gs_color = vs_Color[0];
	EmitVertex();
	
/*	gl_Position = position + vec4(-size, -size * c_fAspect, 0, 0);
	gs_color = vs_Color[0];
	EmitVertex();
	gl_Position = position + vec4( size, -size * c_fAspect, 0, 0);
	gs_color = vs_Color[0];
	EmitVertex();
	gl_Position = position + vec4(-size,  size * c_fAspect, 0, 0);
	gs_color = vs_Color[0];
	EmitVertex();*/
//	gl_Position = position + vec4( size,  size * c_fAspect, 0, 0);
//	gs_color = vs_Color[0];
//	EmitVertex();
	EndPrimitive();
}