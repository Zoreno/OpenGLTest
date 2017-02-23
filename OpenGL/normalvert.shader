#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 vert_normal;

uniform mat4 model;
uniform mat4 transform;

void main()
{
	vert_normal = vertex_normal;

	gl_Position = vec4(vertex_position, 1.0);
}