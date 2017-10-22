#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in float inRotation;
layout(location = 3) in float inSize;

layout(location = 0) out vec4 vs_Position;
layout(location = 1) out vec4 vs_Color;
layout(location = 2) out float vs_Rotation;
layout(location = 3) out float vs_Size;


void main() 
{
	// pass through
    vs_Position = vec4(inPosition, 0.0, 1.0);
    vs_Color = inColor;
	vs_Rotation = inRotation;
	vs_Size = inSize;
}