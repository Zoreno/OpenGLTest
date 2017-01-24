#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default values for camera parameters

const glm::vec3 POSITION = glm::vec3(0.f, 0.f, 0.f);
const glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f);
const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 10.f;
const GLfloat SENSITIVITY = 0.25f;

class Camera
{
public:

	explicit Camera(
		glm::vec3 position = POSITION,
		glm::vec3 up = UP,
		GLfloat yaw = YAW,
		GLfloat pitch = PITCH
		);

	glm::mat4 getViewMatrix() const;

	void processKeyboard(CameraMovement direction, GLfloat timeDelta);
	void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

	glm::vec3 getPosition() const;
	glm::vec3 getFrontVector() const;
	glm::vec3 getUpVector() const;
	glm::vec3 getRightVector() const;
	glm::vec3 getWorldUpVector() const;

	GLfloat getYaw() const;
	GLfloat getPitch() const;
	GLfloat getMovementSpeed() const;
	GLfloat getMouseSensitivity() const;

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;

	void updateCameraVectors();
};