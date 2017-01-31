#version 330 core

in vec3 out_Color;
in vec2 out_TexCoords;

out vec4 frag_color;

uniform sampler2D texUnit;

void main(void)
{
	frag_color = texture(texUnit, out_TexCoords);
}