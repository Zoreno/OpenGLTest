#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_TexCoords;

out vec3 out_Color;
out vec2 out_TexCoords;

uniform mat4 myMatrix;

void main(void)
{
	out_Color = in_Color;
	out_TexCoords = in_TexCoords;
	gl_Position = myMatrix*vec4(in_Position, 1.0);
}