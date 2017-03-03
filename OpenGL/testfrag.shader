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

	float constant;
	float linear;
	float quadratic;

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

#define MAX_NUM_LIGHTS 64

uniform int nLights;
uniform Light light[MAX_NUM_LIGHTS];

uniform sampler2D texUnit;
uniform sampler2D texUnit2;
uniform samplerCube depthMap;
uniform float far_plane;
uniform float alpha;

float ShadowCalculation(vec3 fragPos, int i)
{
	vec3 fragToLight = fragPos - light[i].position;

	float currentDepth = length(fragToLight);

	float cosTheta = clamp(dot(normalize(normal), normalize(-fragToLight)), 0, 1);

	float bias = 0.005*tan(acos(cosTheta));
	bias = clamp(bias, 0, 0.01);
	
	float shadow = 0.0;
	float samples = 4.0;
	float offset = 0.1;

	for (float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for (float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for (float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r;
				closestDepth *= far_plane;   // Undo mapping [0;1]
				if (currentDepth - bias > closestDepth)
					shadow += 1.0;
			}
		}
	}

	shadow /= (samples * samples * samples);

	return shadow;
}

void main()
{
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(view_pos - FragPos);

	vec3 result;
	for (int i = 0; i < nLights; ++i)
	{
		// Ambient color calculation
		vec3 ambient = light[i].ambient*material.ambient;

		// Diffuse color calculation
		
		vec3 light_dir = normalize(light[i].position - FragPos);
		float diff = max(dot(norm, light_dir), 0.0);
		vec3 diffuse = light[i].diffuse * (diff * material.diffuse);

		// Specular light calculation
		
		vec3 reflect_dir = reflect(-light_dir, norm);
		float distance = length(light[i].position - FragPos);

		float attenuation = 1.0f / (light[i].constant + light[i].linear * distance +
			light[i].quadratic * (distance * distance));

		float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
		vec3 specular = light[i].specular * (spec * material.specular);

		float shadow = ShadowCalculation(FragPos,i);

		// Add them
		result += (ambient + (1.0-shadow)*(diffuse + specular))*attenuation;
	}

	vec4 objColor1 = texture(texUnit, texCoords);
	vec4 objColor2 = texture(texUnit2, texCoords);

	vec3 up = vec3(0, 1, 0);

	vec4 objColor = mix(objColor1, objColor2, pow(clamp(dot(up, norm),0,1), 1));
	// Labb 2 Uppgift 1
	// frag_color = vec4(texCoords, 0.0f, 1.0f);

	// Labb 2 Uppgift 2
	//frag_color = objColor;

	// Labb 2 Uppgift 5
	
	objColor.a = objColor.a*alpha;

	frag_color = objColor * vec4(result, 1.f);
}