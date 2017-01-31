#version 330 core

in vec3 out_Color;

out vec4 frag_color;

void main(void)
{
	frag_color = vec4(out_Color, 1.0);
}