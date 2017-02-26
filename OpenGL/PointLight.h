#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Must match shader representation of light

class PointLight
{
public:
	PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

	glm::vec3 getPosition();
	glm::vec3 getAmbient();
	glm::vec3 getDiffuse();
	glm::vec3 getSpecular();

private:
	// Position info
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	// Color info
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
