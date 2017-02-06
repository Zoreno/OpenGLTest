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

void main()
{
	// Ambient color calculation
	vec3 ambient = vec3(0.2,0.2,0.2);

	// Diffuse color calculation
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light.position - FragPos);
	float diff = max(dot(norm, light_dir), 0.0);
	//vec3 diffuse = light.diffuse * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * diff;

	// Specular light calculation
	vec3 view_dir = normalize(view_pos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	// Multiply them
	vec3 result = (ambient + diffuse + specular);

	vec4 objColor = texture(texUnit, texCoords);

	// Labb 2 Uppgift 1
	// frag_color = vec4(texCoords, 0.0f, 1.0f);

	// Labb 2 Uppgift 2
	//frag_color = objColor;

	// Labb 2 Uppgift 5
	frag_color = objColor * diffuse;
}