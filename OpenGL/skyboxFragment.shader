#version 330 core

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

out vec4 frag_color;

uniform float time;

uniform sampler2D texUnit;

void main()
{
	vec4 objColor = texture(texUnit, texCoords);

	frag_color = objColor.zyxw;
}