#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

out vec4 frag_color;

uniform float time;

uniform vec3 view_pos;

uniform Material material;
uniform Light light;

uniform sampler2D texUnit;
uniform sampler2D texUnit2;

void main()
{
	// Ambient color calculation
	vec3 ambient = light.ambient*material.ambient;

	// Diffuse color calculation
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light.position - FragPos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular light calculation
	vec3 view_dir = normalize(view_pos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	// Add them
	vec3 result = (ambient + diffuse + specular);

	vec4 objColor1 = texture(texUnit, texCoords);
	vec4 objColor2 = texture(texUnit2, texCoords);

	vec3 up = vec3(0, 1, 0);

	vec4 objColor = mix(objColor1, objColor2, pow(clamp(dot(up, norm),0,1), 2));
	// Labb 2 Uppgift 1
	// frag_color = vec4(texCoords, 0.0f, 1.0f);

	// Labb 2 Uppgift 2
	//frag_color = objColor;

	// Labb 2 Uppgift 5
	frag_color = objColor * vec4(result, 1.f);
}