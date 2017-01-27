#include "PointLight.h"

PointLight::PointLight(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
	: position(position),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{
}

glm::vec3 PointLight::getPosition()
{
	return position;
}

glm::vec3 PointLight::getAmbient()
{
	return ambient;
}

glm::vec3 PointLight::getDiffuse()
{
	return diffuse;
}

glm::vec3 PointLight::getSpecular()
{
	return specular;
}
