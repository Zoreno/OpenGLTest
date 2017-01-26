#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "ShaderProgram.h"
#include "loadobj.h"
#include "Camera.h"
#include "TGA.h"
#include "Texture2D.h"

using namespace glm;

void GLFWError(int errorCode, const char* message)
{
	std::cerr << "Recieved error " << errorCode << ":" << message << std::endl;
}

bool recompileShaders(ShaderProgram& currentShader)
{
	ShaderProgram shaderProgram{ "testvert.shader", "testfrag.shader" };
	try
	{
		shaderProgram.compile();

		shaderProgram.bindAttribLocation(0, "vertex_position");
		shaderProgram.bindAttribLocation(1, "vertex_normal");
		shaderProgram.bindAttribLocation(2, "vertex_texture_coordinates");

		shaderProgram.link();

		shaderProgram.use();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << "Error recompiling shader. No action taken." << std::endl << ex.what() << std::endl;
		return false;
	}

	std::cout << "Shader recompilation successful" << std::endl;

	currentShader = std::move(shaderProgram);

	return true;
}

int main()
{
	try
	{


		//============================================================================
		// Init
		//============================================================================

		Window window{ 1024, 768, "Test" };

		//============================================================================
		// Vertex buffer setup
		//============================================================================

		Model* m;
		m = LoadModel("bunnyplus.obj");

		GLuint bunnyVertexArrayObjID; // VAO
		GLuint bunnyVertexBufferObjID; // VBO
		GLuint bunnyIndexBufferObjID; // EBO
		GLuint bunnyNormalBufferObjID; // NBO 
		GLuint bunnyTexCoordsBufferObjID; // TBO

		glGenVertexArrays(1, &bunnyVertexArrayObjID);
		glGenBuffers(1, &bunnyVertexBufferObjID);
		glGenBuffers(1, &bunnyIndexBufferObjID);
		glGenBuffers(1, &bunnyNormalBufferObjID);
		glGenBuffers(1, &bunnyTexCoordsBufferObjID);

		glBindVertexArray(bunnyVertexArrayObjID);

		// VBO for vertex data
		glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// VBO for normal data
		glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices * 3 * sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices * sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

		if (m->texCoordArray != nullptr)
		{
			std::cout << "Model has texture info" << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordsBufferObjID);
			glBufferData(GL_ARRAY_BUFFER, m->numVertices * 2 * sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);
	}

		//============================================================================
		// Textures
		//============================================================================

		glActiveTexture(GL_TEXTURE0);

		Texture2D texture{ "conc.tga" };

#if 0
		GLuint textureID;
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &textureID);
		TGA tgaFile("maskros512.tga");
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, tgaFile.hasAlpha() ? GL_RGBA : GL_RGB, tgaFile.getWidth(), tgaFile.getHeight(), 0, tgaFile.hasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tgaFile.getPixels().data());
		glGenerateMipmap(GL_TEXTURE_2D);
#endif 
		//============================================================================
		// Shaders
		//============================================================================

		ShaderProgram shaderProgram{ "testvert.shader", "testfrag.shader" };
		try
		{
			shaderProgram.compile();

			shaderProgram.bindAttribLocation(0, "vertex_position");
			shaderProgram.bindAttribLocation(1, "vertex_normal");
			shaderProgram.bindAttribLocation(2, "vertex_texture_coordinates");

			shaderProgram.link();

			shaderProgram.use();
		}
		catch (const ShaderProgramException& ex)
		{
			std::cerr << ex.what() << std::endl;
			glfwTerminate();
			return -1;
		}


		glfwSetErrorCallback(GLFWError);

		//============================================================================
		// Final Setup
		//============================================================================

		glClearColor(0.f, 0.f, 0.f, 1.0f);

		GLfloat time = (GLfloat)glfwGetTime();
		GLfloat timeElapsed = 0.f;
		GLuint frames = 0;

		glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Set render wireframe mode

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		Camera camera{ glm::vec3(0.f,0.f,4.f) };

		bool firstMouse = true;
		GLfloat lastX{ 0.f };
		GLfloat lastY{ 0.f };

		bool forwardKeyPressed{ false };
		bool backwardKeyPressed{ false };
		bool leftKeyPressed{ false };
		bool rightKeyPressed{ false };

		while (!window.shouldClose())
		{
			// Clear screen and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//============================================================================
			// Dynamic Uniforms
			//============================================================================

			// Recalculate time and FPS counter

			GLfloat oldTime = time;
			time = (GLfloat)glfwGetTime();
			GLfloat timeDelta = (time - oldTime);
			timeElapsed += timeDelta;
			++frames;

			if (timeElapsed > 1.f)
			{
				timeElapsed -= 1.f;
				std::string newTitle = std::to_string(frames) + std::string{ " FPS" };
				window.setTitle(newTitle);
				frames = 0;
			}

			// Handle window events
			WindowEvent ev;
			if (window.pollEvent(ev))
			{
				switch (ev.type)
				{
				case KEY_EVENT:
					if (ev.key.key == GLFW_KEY_W)
					{
						if (ev.key.action == GLFW_PRESS)
							forwardKeyPressed = true;
						else if (ev.key.action == GLFW_RELEASE)
							forwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_S)
					{
						if (ev.key.action == GLFW_PRESS)
							backwardKeyPressed = true;
						else if (ev.key.action == GLFW_RELEASE)
							backwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_A)
					{
						if (ev.key.action == GLFW_PRESS)
							leftKeyPressed = true;
						else if (ev.key.action == GLFW_RELEASE)
							leftKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_D)
					{
						if (ev.key.action == GLFW_PRESS)
							rightKeyPressed = true;
						else  if (ev.key.action == GLFW_RELEASE)
							rightKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_F5)
					{
						if (ev.key.action == GLFW_PRESS)
							recompileShaders(shaderProgram);
					}
					break;
				case MOUSE_EVENT:
				{
					if (firstMouse)
					{
						lastX = ev.mouse.posx;
						lastY = ev.mouse.posy;
						firstMouse = false;
					}

					GLfloat xOffset = ev.mouse.posx - lastX;
					GLfloat yOffset = lastY - ev.mouse.posy;

					lastX = ev.mouse.posx;
					lastY = ev.mouse.posy;

					camera.processMouseMovement(xOffset, yOffset);
				}
				break;
				default:
					break;
				}
			}

			if (forwardKeyPressed)
				camera.processKeyboard(FORWARD, timeDelta);
			if (backwardKeyPressed)
				camera.processKeyboard(BACKWARD, timeDelta);
			if (leftKeyPressed)
				camera.processKeyboard(LEFT, timeDelta);
			if (rightKeyPressed)
				camera.processKeyboard(RIGHT, timeDelta);

			glm::vec3 axis_y(0.f, 1.f, 0.f);
			glm::mat4 anim = glm::rotate(mat4{ 1.f }, glm::radians(time*50.f), axis_y);

			glm::mat4 model = anim;
			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = glm::perspective(radians(45.f), (GLfloat)window.getDimensions().x / (GLfloat)window.getDimensions().y, 0.1f, 100.f);

			mat4 trans = projection*view*model;

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "transform"), 1, GL_FALSE, value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "model"), 1, GL_FALSE, value_ptr(model));
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "time"), time);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "view_pos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

			// Light uniforms

			//glm::vec3 lightPos(4.f, 3.f, 3.f);
			glm::vec3 lightPos = camera.getPosition();
			glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
			glm::vec3 diffuseColor = lightColor; // Decrease the influence
			glm::vec3 ambientColor = diffuseColor; // Low influence

			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.position"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.specular"), 1.0f, 1.0f, 1.0f);

			// Material Uniforms
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.ambient"), 0.0215f, 0.1745f, 0.0215f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.diffuse"), 0.07568f, 0.61424f, 0.07568f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.specular"), 0.633f, 0.727811f, 0.633f);
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.shininess"), 0.6f*128.f);

			glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "texUnit"), 0);

			glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
			glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

			window.display();
		}

		DisposeModel(m);

		glfwTerminate();
}
	catch (const std::exception& ex)
	{
		std::cerr << "[FATAL] Caught exception at main level:" << std::endl << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "[FATAL] Caught unknown exception at main level" << std::endl;
	}

	return 0;
}