#include <iostream>
#include <string>
#include <fstream>

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

#include "ConfigFile.h"

#include <rapidxml/rapidxml.hpp>
#include "Utils.h"
#include "Collada.h"

#include "RawModel.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "WaveFile.h"
#include "AudioSource.h"
#include "AudioListener.h"

#include <list>
#include <thread>

using namespace glm;

void GLFWError(int errorCode, const char* message)
{
	std::cerr << "Recieved error " << errorCode << ":" << message << std::endl;
}

bool recompileShaders(ShaderProgram& currentShader)
{
	/*
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
	*/

	currentShader.compile();

	currentShader.bindAttribLocation(0, "vertex_position");
	currentShader.bindAttribLocation(1, "vertex_normal");
	currentShader.bindAttribLocation(2, "vertex_texture_coordinates");

	currentShader.link();

	currentShader.use();

	return true;
}

void reloadConfig(ConfigFile& file)
{
	file.parse("test.cfg");
}

int labb1_cube()
{
	GLfloat vertices[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	GLfloat colors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
	};

	Window window{ 1024, 728, "Labb 1" };
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint vertexVBO;

	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLuint colorVBO;

	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	ShaderProgram program{ "labb1-1.vert", "labb1-1.frag" };
	try
	{
		program.compile();
		program.bindAttribLocation(0, "in_Position");
		program.bindAttribLocation(1, "in_Color");
		program.link();

		program.use();

	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}

	GLfloat time = (GLfloat)glfwGetTime();
	GLfloat timeElapsed = 0.f;
	GLuint frames = 0;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	while (!window.shouldClose())
	{
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		WindowEvent ev;
		while (window.pollEvent(ev));

		glm::mat4 mat{ 1.f };

		mat = glm::scale(mat, glm::vec3(0.5f, 0.5f, 0.5f));
		mat = glm::rotate(mat, glm::radians(time * 50.f), glm::normalize(glm::vec3(0.f, 1.f, 1.f)));

		glUniformMatrix4fv(glGetUniformLocation(program.getShaderProgramHandle(), "myMatrix"), 1, GL_FALSE, glm::value_ptr(mat));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		window.display();
	}
}

int labb1_triangle()
{
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0,
		-0.5f, 0.5f, 0,
		0.5f, -0.5f, 0
	};

	GLfloat colors[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	};

	GLfloat myMatrix[] = {
		1.f,0.f,0.f, 0.5f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	};

	GLfloat texCoords[] = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 0.f
	};

	WindowSettings settings = getDefaultWindowSettings();

	settings.maximized = GLFW_TRUE;
	settings.vSync = GLFW_FALSE;

	Window window{ 1024, 728, "Labb 1" , settings };
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint vertexVBO;

	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLuint colorVBO;

	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint textureVBO;

	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	Texture2D tex{ "maskros512.tga" };

	ShaderProgram program{ "labb1-1.vert", "labb1-1.frag" };
	try
	{
		program.compile();
		program.bindAttribLocation(0, "in_Position");
		program.bindAttribLocation(1, "in_Color");
		program.bindAttribLocation(2, "in_TexCoords");
		program.link();

		program.use();

	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}

	GLfloat time = (GLfloat)glfwGetTime();
	GLfloat timeElapsed = 0.f;
	GLuint frames = 0;

	while (!window.shouldClose())
	{
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		WindowEvent ev;
		while (window.pollEvent(ev));

		glm::mat4 mat{ 1.f };

		mat = glm::rotate(mat, glm::radians(time * 50.f), glm::vec3(0.f, 0.f, 1.f));

		glUniformMatrix4fv(glGetUniformLocation(program.getShaderProgramHandle(), "myMatrix"), 1, GL_FALSE, glm::value_ptr(mat));

		program.uploadUniform("texUnit", 0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.display();
	}
}

int labb1_bunny()
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

#if 0

		Model* m;
		m = LoadModel("bunnyplus.obj");

		GLuint bunnyVertexArrayObjID;

		GLuint bunnyVertexBufferObjID;
		GLuint bunnyIndexBufferObjID;
		GLuint bunnyNormalBufferObjID;
		GLuint bunnyTexCoordsBufferObjID;

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

#endif

		RawModel rawModel{ "bunnyplus.obj" };

		//============================================================================
		// Textures
		//============================================================================

		glActiveTexture(GL_TEXTURE0);

		// TODO: Encapsulate for guarantee of memory release
		TextureMap textures;
		textures.emplace("Concrete", new Texture2D{ "conc.tga" });
		textures.emplace("Flower", new Texture2D{ "maskros512.tga" });

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
				case EventType::KEY_EVENT:
					if (ev.key.key == GLFW_KEY_W)
					{
						if (ev.key.action == Action::PRESS)
							forwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							forwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_S)
					{
						if (ev.key.action == Action::PRESS)
							backwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							backwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_A)
					{
						if (ev.key.action == Action::PRESS)
							leftKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							leftKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_D)
					{
						if (ev.key.action == Action::PRESS)
							rightKeyPressed = true;
						else  if (ev.key.action == Action::RELEASE)
							rightKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_F5)
					{
						if (ev.key.action == Action::PRESS)
							recompileShaders(shaderProgram);
					}
					break;
				case EventType::MOUSE_MOVED_EVENT:
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

			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);
			shaderProgram.uploadUniform("time", time);

			shaderProgram.uploadUniform("view_pos", camera.getPosition());

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

			//glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "texUnit"), 0);

			shaderProgram.uploadUniform("texUnit", 0);

			textures.at("Concrete")->bind(0);
#if 0
			glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
			glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

#endif

			rawModel.draw();

			window.display();
		}
#if 0
		DisposeModel(m);
#endif

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

int labb2_bunny()
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
		m = LoadModelPlus("bunnyplus.obj");

		Model* m2;
		m2 = LoadModelPlus("cubeplus.obj");

		//============================================================================
		// Textures
		//============================================================================

		glActiveTexture(GL_TEXTURE0);

		// TODO: Encapsulate for guarantee of memory release
		TextureMap textures;
		textures.emplace("Concrete", new Texture2D{ "conc.tga" });
		textures.emplace("Flower", new Texture2D{ "maskros512.tga" });

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

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

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
				case EventType::KEY_EVENT:
					if (ev.key.key == GLFW_KEY_W)
					{
						if (ev.key.action == Action::PRESS)
							forwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							forwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_S)
					{
						if (ev.key.action == Action::PRESS)
							backwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							backwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_A)
					{
						if (ev.key.action == Action::PRESS)
							leftKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							leftKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_D)
					{
						if (ev.key.action == Action::PRESS)
							rightKeyPressed = true;
						else  if (ev.key.action == Action::RELEASE)
							rightKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_F5)
					{
						if (ev.key.action == Action::PRESS)
							recompileShaders(shaderProgram);
					}
					break;
				case EventType::MOUSE_MOVED_EVENT:
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

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = glm::perspective(radians(45.f), (GLfloat)window.getDimensions().x / (GLfloat)window.getDimensions().y, 0.1f, 100.f);

			shaderProgram.uploadUniform("time", time);

			shaderProgram.uploadUniform("view_pos", camera.getPosition());

			// Light uniforms

			glm::vec3 lightPos(4.f, 3.f, 3.f);
			//glm::vec3 lightPos = camera.getPosition();
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

			//glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "texUnit"), 0);


			shaderProgram.uploadUniform("texUnit", 0);
			textures.at("Concrete")->bind(0);

			glm::vec3 axis_y(0.f, 1.f, 0.f);
			glm::mat4 anim = glm::rotate(mat4{ 1.f }, glm::radians(time*50.f), axis_y);
			glm::mat4 model = glm::translate(anim, glm::vec3(-2, 0, 0));
			mat4 trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(m, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			model = glm::translate(anim, glm::vec3(2, 0, 0));

			trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			textures.at("Flower")->bind(0);

			DrawModel(m2, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

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

int labb3_windmill()
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
		m = LoadModelPlus("windmill-walls.obj");

		Model* m2;
		m2 = LoadModelPlus("windmill-roof.obj");

		Model* m3;
		m3 = LoadModelPlus("windmill-balcony.obj");

		Model* m4;
		m4 = LoadModelPlus("blade.obj");

		Model* ground;
		ground = LoadModelPlus("ground.obj");

		Model* skybox;
		skybox = LoadModelPlus("skybox.obj");

		Model* bunny;
		bunny = LoadModelPlus("bunnyplus.obj");

		//============================================================================
		// Textures
		//============================================================================

		glActiveTexture(GL_TEXTURE0);

		// TODO: Encapsulate for guarantee of memory release
		TextureMap textures;
		textures.emplace("Concrete", new Texture2D{ "conc.tga" });
		textures.emplace("Flower", new Texture2D{ "maskros512.tga" });
		textures.emplace("ground", new Texture2D{ "grass.tga" });
		textures.emplace("skybox", new Texture2D("SkyBox512.tga"));

		//============================================================================
		// Shaders
		//============================================================================

		ShaderProgram shaderProgram{ "testvert.shader", "testfrag.shader" };
		try
		{
			shaderProgram.use();
			shaderProgram.compile();

			shaderProgram.bindAttribLocation(0, "vertex_position");
			shaderProgram.bindAttribLocation(1, "vertex_normal");
			shaderProgram.bindAttribLocation(2, "vertex_texture_coordinates");

			shaderProgram.link();
		}
		catch (const ShaderProgramException& ex)
		{
			std::cerr << ex.what() << std::endl;
			glfwTerminate();
			return -1;
		}

		ShaderProgram skyboxShaderProgram("skyboxVertex.shader", "skyboxFragment.shader");
		try
		{
			skyboxShaderProgram.use();
			skyboxShaderProgram.compile();

			skyboxShaderProgram.bindAttribLocation(0, "vertex_position");
			skyboxShaderProgram.bindAttribLocation(1, "vertex_normal");
			skyboxShaderProgram.bindAttribLocation(2, "vertex_texture_coordinates");

			skyboxShaderProgram.link();
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

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		Camera camera{ glm::vec3(0.f,2.f,4.f) };

		bool firstMouse = true;
		GLfloat lastX{ 0.f };
		GLfloat lastY{ 0.f };

		bool forwardKeyPressed{ false };
		bool backwardKeyPressed{ false };
		bool leftKeyPressed{ false };
		bool rightKeyPressed{ false };
		bool spacePressed{ false };
		bool shiftPressed{ false };

		ConfigFile positions{ "test.cfg" };

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
				case EventType::KEY_EVENT:
					if (ev.key.key == GLFW_KEY_W)
					{
						if (ev.key.action == Action::PRESS)
							forwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							forwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_S)
					{
						if (ev.key.action == Action::PRESS)
							backwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							backwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_A)
					{
						if (ev.key.action == Action::PRESS)
							leftKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							leftKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_D)
					{
						if (ev.key.action == Action::PRESS)
							rightKeyPressed = true;
						else  if (ev.key.action == Action::RELEASE)
							rightKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_SPACE)
					{
						if (ev.key.action == Action::PRESS)
							spacePressed = true;
						else  if (ev.key.action == Action::RELEASE)
							spacePressed = false;
					}
					if (ev.key.key == GLFW_KEY_LEFT_SHIFT)
					{
						if (ev.key.action == Action::PRESS)
							shiftPressed = true;
						else  if (ev.key.action == Action::RELEASE)
							shiftPressed = false;
					}
					if (ev.key.key == GLFW_KEY_F5)
					{
						if (ev.key.action == Action::PRESS)
							recompileShaders(shaderProgram);
					}

					if (ev.key.key == GLFW_KEY_F6)
					{
						if (ev.key.action == Action::PRESS)
							reloadConfig(positions);
					}
					break;
				case EventType::MOUSE_MOVED_EVENT:
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
			if (spacePressed) {
				if (shiftPressed)
					camera.processKeyboard(ABSDOWN, timeDelta);
				else
					camera.processKeyboard(ABSUP, timeDelta);
			}

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = glm::perspective(radians(45.f), (GLfloat)window.getDimensions().x / (GLfloat)window.getDimensions().y, 0.1f, 100.f);

			shaderProgram.uploadUniform("time", time);
			shaderProgram.uploadUniform("view_pos", camera.getPosition());

			// Light uniforms

			glm::vec3 lightPos(4.f, 3.f, 3.f);
			//glm::vec3 lightPos = camera.getPosition();
			glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
			glm::vec3 diffuseColor = lightColor*0.7f; // Decrease the influence
			glm::vec3 ambientColor = diffuseColor*0.5f; // Low influence

			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.position"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.specular"), 1.0f, 1.0f, 1.0f);

			// Material Uniforms
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.ambient"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.diffuse"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.specular"), 1.f, 1.f, 1.f);
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.shininess"), 0.6f*128.f);


			skyboxShaderProgram.use();
			glm::mat4 skyboxModel = glm::translate(glm::mat4{ 1.f }, camera.getPosition());

			glm::mat4 skyboxView = camera.getViewMatrix();

			skyboxView[0][3] = 0;
			skyboxView[1][3] = 0;
			skyboxView[2][3] = 0;

			mat4 skyboxTransform = projection*skyboxView*skyboxModel;
			skyboxShaderProgram.uploadUniform("transform", skyboxTransform);
			skyboxShaderProgram.uploadUniform("model", skyboxModel);
			skyboxShaderProgram.uploadUniform("texUnit", 0);

			textures.at("skybox")->bind(0);

			glDisable(GL_DEPTH_TEST);

			glCullFace(GL_FRONT);

			DrawModel(skybox, skyboxShaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			glCullFace(GL_BACK);

			glEnable(GL_DEPTH_TEST);

			shaderProgram.use();

			textures.at("Flower")->bind(1);

			glm::vec3 windmill_position{ 10.f,0.f,sin(time) };

			shaderProgram.uploadUniform("texUnit", 0);
			shaderProgram.uploadUniform("texUnit2", 1);

			textures.at("Concrete")->bind(0);

			glm::mat4 model = glm::translate(glm::mat4{ 1.f }, windmill_position);
			glm::mat4 trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(m, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			textures.at("Flower")->bind(0);

			DrawModel(m2, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			DrawModel(m3, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			glm::vec3 trans_vec{ positions.get<float>("centerX"),positions.get<float>("centerY"), positions.get<float>("centerZ") };

			glm::mat4 translation = glm::translate(model, trans_vec);

			glm::vec3 axis_x(1.f, 0.f, 0.f);

			glm::mat4 timeRotation = glm::rotate(glm::mat4{ 1.f }, glm::radians(time*10.f), axis_x);

			for (int i{ 0 }; i < 4; ++i)
			{
				glm::mat4 localRotation = glm::rotate(glm::mat4{ 1.f }, glm::radians(90.f*i), axis_x);
				glm::mat4 model = translation*localRotation*timeRotation;

				mat4 trans = projection*view*model;
				shaderProgram.uploadUniform("transform", trans);
				shaderProgram.uploadUniform("model", model);

				DrawModel(m4, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");
			}

			textures.at("ground")->bind(0);

			model = mat4{ 1.f };
			trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(ground, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			model = glm::translate(glm::scale(glm::mat4{ 1.f }, glm::vec3(3.0f, 3.0f, 3.0f)), glm::vec3{ 0.f,0.5f,0.f });
			trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			textures.at("Concrete")->bind(0);

			DrawModel(bunny, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			window.display();
		}

		DisposeModel(m);
		DisposeModel(m2);
		DisposeModel(m3);
		DisposeModel(m4);
		DisposeModel(ground);
		DisposeModel(skybox);
		DisposeModel(bunny);

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

int labb3_world()
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
		m = LoadModelPlus("bunnyplus.obj");

		Model* m2;
		m2 = LoadModelPlus("cubeplus.obj");

		Model* m3;
		m3 = LoadModelPlus("ground.obj");

		//============================================================================
		// Textures
		//============================================================================

		glActiveTexture(GL_TEXTURE0);

		// TODO: Encapsulate for guarantee of memory release
		TextureMap textures;
		textures.emplace("Concrete", new Texture2D{ "conc.tga" });
		textures.emplace("Flower", new Texture2D{ "maskros512.tga" });
		textures.emplace("ground", new Texture2D{ "grass.tga" });

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

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		Camera camera{ glm::vec3(0.f,2.f,4.f) };

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
				case EventType::KEY_EVENT:
					if (ev.key.key == GLFW_KEY_W)
					{
						if (ev.key.action == Action::PRESS)
							forwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							forwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_S)
					{
						if (ev.key.action == Action::PRESS)
							backwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							backwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_A)
					{
						if (ev.key.action == Action::PRESS)
							leftKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							leftKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_D)
					{
						if (ev.key.action == Action::PRESS)
							rightKeyPressed = true;
						else  if (ev.key.action == Action::RELEASE)
							rightKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_F5)
					{
						if (ev.key.action == Action::PRESS)
							recompileShaders(shaderProgram);
					}
					break;
				case EventType::MOUSE_MOVED_EVENT:
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

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = glm::perspective(radians(45.f), (GLfloat)window.getDimensions().x / (GLfloat)window.getDimensions().y, 0.1f, 100.f);

			shaderProgram.uploadUniform("time", time);

			shaderProgram.uploadUniform("view_pos", camera.getPosition());

			// Light uniforms

			glm::vec3 lightPos(4.f, 3.f, 3.f);
			//glm::vec3 lightPos = camera.getPosition();
			glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
			glm::vec3 diffuseColor = lightColor*0.7f; // Decrease the influence
			glm::vec3 ambientColor = diffuseColor*0.3f; // Low influence

			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.position"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.specular"), 1.0f, 1.0f, 1.0f);

			// Material Uniforms
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.ambient"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.diffuse"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.specular"), 1.f, 1.f, 1.f);
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.shininess"), 0.6f*128.f);

			//glUniform1i(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "texUnit"), 0);


			shaderProgram.uploadUniform("texUnit", 0);
			textures.at("Concrete")->bind(0);

			glm::vec3 axis_y(0.f, 1.f, 0.f);
			glm::mat4 anim = glm::rotate(mat4{ 1.f }, glm::radians(time*50.f), axis_y);
			glm::mat4 model = glm::translate(anim, glm::vec3(-2, 0.5, 0));
			mat4 trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(m, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			textures.at("Flower")->bind(0);

			model = glm::translate(anim, glm::vec3(2, 0.5, 0));
			trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(m2, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			textures.at("ground")->bind(0);

			model = mat4{ 1.f };
			trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(m3, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

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

int collada_test()
{
	collada::COLLADA colladaStruct;
	try
	{
		loadCOLLADA("cubewithtexture.dae", &colladaStruct);

		for (collada::Geometry& geom : colladaStruct.geometryLibrary.geometries)
		{
			std::cout << "Geom ID:" << geom.ID << ". Geom Name:" << geom.name << std::endl;

			collada::Mesh& mesh = geom.mesh;

			for (collada::Source& source : mesh.sources)
			{
				std::cout << "Source ID:" << source.ID << std::endl;
			}
		}
	}
	catch (const rapidxml::parse_error& ex)
	{
		std::cerr << "What:" << ex.what() << std::endl << "Where:" << ex.where<char>() << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "What:" << ex.what() << std::endl;
	}

	return 0;
}

int config_test()
{
	float f1, f2;

	int i1;

	ConfigFile configFile{ "test.cfg" };

	f1 = configFile.get<float>("testFloat1");
	f2 = configFile.get<float>("testFloat2");

	std::cout << f1 + f2 << std::endl;

	i1 = configFile.get<int>("testInt");

	std::cout << i1 << std::endl;

	return 0;
}

int openAL_test()
{
	// Dummy read to flush error code.
	alGetError();

	// Open a device
	ALCdevice* device;
	ALCcontext* context;
	ALenum error;

	device = alcOpenDevice(NULL);

	if (!device)
	{
		std::cout << "No Device" << std::endl;
		return -1;
	}

	context = alcCreateContext(device, NULL);

	alcMakeContextCurrent(context);

	if (!context)
	{
		std::cout << "No Context" << std::endl;
		return -1;
	}

#define NUM_BUFFERS 1
#define NUM_SOURCES 1

	ALuint buffers[NUM_BUFFERS];
	ALuint sources[NUM_SOURCES];

	// Generate and fill buffers
	alGenBuffers(NUM_BUFFERS, buffers);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '1' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		return -1;
	}

	WaveFile blip{ "Music.wav" , true };

	std::cout << "Source Size: " << blip.getSize() << std::endl;
	std::cout << "Source Freq: " << blip.getFrequency() << std::endl;

	std::cout << "Source format: ";

	switch (blip.getFormat())
	{
	case AL_FORMAT_STEREO16:
		std::cout << "AL_FORMAT_STEREO16";
		break;
	case AL_FORMAT_STEREO8:
		std::cout << "AL_FORMAT_STEREO8";
		break;
	case AL_FORMAT_MONO16:
		std::cout << "AL_FORMAT_MONO16";
		break;
	case AL_FORMAT_MONO8:
		std::cout << "AL_FORMAT_MONO8";
		break;
	}

	std::cout << std::endl;

	alBufferData(buffers[0], blip.getFormat(), blip.getData(), blip.getSize(), blip.getFrequency());
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '2' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		return -1;
	}

	// Generate and assign sources

	alGenSources(NUM_SOURCES, sources);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '3' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	alSourcei(sources[0], AL_BUFFER, buffers[0]);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '4' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	// Position source

	ALfloat sourcePos[3] = { 0.f,0.f,0.f };
	ALfloat sourceVel[3] = { 0.f,0.f,0.f };

	alSourcefv(sources[0], AL_POSITION, sourcePos);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '5' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	alSourcefv(sources[0], AL_VELOCITY, sourceVel);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '6' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	alSourcef(sources[0], AL_PITCH, 1);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '8' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	alSourcef(sources[0], AL_GAIN, 1);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '9' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	alSourcei(sources[0], AL_LOOPING, blip.getLoop());
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '10' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}
	// Position Listener

	ALfloat listenerPos[3] = { 0.f,0.f,0.f };
	ALfloat listenerVel[3] = { 0.f,0.f,0.f };

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerf(AL_GAIN, 1);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '11' << alcGetString(device, error) << std::endl;
		alDeleteBuffers(NUM_BUFFERS, buffers);
		alDeleteSources(NUM_SOURCES, sources);
		return -1;
	}

	// Play Source

	std::cout << "p - play" << std::endl << "s - stop" << std::endl << "q - quit" << std::endl;
	while (true)
	{
		char c;
		std::cin >> c;
		if (c == 'q')
			break;
		if (c == 'p')
			alSourcePlay(sources[0]);
		if (c == 's')
			alSourceStop(sources[0]);
	}

	alDeleteSources(NUM_SOURCES, sources);
	alDeleteBuffers(NUM_BUFFERS, buffers);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '12' << alcGetString(device, error) << std::endl;
		return -1;
	}

	return 0;

	// Device
	// Context
	// Source - Done
	// Buffer - Done
	// Listener - Done
}

int openAL_test2()
{
	// Dummy read to flush error code.
	alGetError();

	// Open a device
	ALCdevice* device;
	ALCcontext* context;
	ALenum error;

	device = alcOpenDevice(NULL);

	if (!device)
	{
		std::cout << "No Device" << std::endl;
		return -1;
	}

	context = alcCreateContext(device, NULL);

	alcMakeContextCurrent(context);

	if (!context)
	{
		std::cout << "No Context" << std::endl;
		return -1;
	}


	{
		AudioSource source{ "Music.wav", true };
		AudioListener listener{};

		// Play Source

		std::cout << "p - play" << std::endl << "s - stop" << std::endl << "r - rewind" << std::endl << "q - quit" << std::endl;
		while (true)
		{
			char c;
			std::cin >> c;
			if (c == 'q')
				break;
			if (c == 'p')
				source.play();
			if (c == 's')
				source.stop();
			if (c == 'r')
				source.rewind();
		}

	}

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cerr << '12' << alcGetString(device, error) << std::endl;
		return -1;
	}

	return 0;

	// Device
	// Context
	// Source - Done
	// Buffer - Done
	// Listener - Done
}

int WAV_test()
{
	char* source = "Jump3.wav";

	std::ifstream fStream{ source };

	if (!fStream.is_open())
	{
		std::cerr << "Could not open file" << std::endl;
		return -1;
	}

	WAVHeader header;

	fStream.read(reinterpret_cast<char*>(&header), sizeof(header));

	// Check that file is in a RIFF chunk

	if (strncmp(header.chunkID, "RIFF", 4) != 0)
	{
		std::cerr << "File not RIFF format" << std::endl;
		return -1;
	}

	// Check that file is in WAVE format

	if (strncmp(header.format, "WAVE", 4) != 0)
	{
		std::cerr << "File Not WAVE Format" << std::endl;
		return -1;
	}

	if (strncmp(header.subChunk1ID, "fmt ", 4) != 0)
	{
		std::cerr << "Format sub-chunk expected" << std::endl;
		return -1;
	}

	std::cout << "Successully read file " << source << " of size " << header.chunkSize << std::endl;

	std::cout << "Format Header Data:" << std::endl;

	std::cout << "Audio Format: " << header.audioFormat << std::endl;
	std::cout << "Number of Channels: " << header.numChannels << std::endl;
	std::cout << "Sample Rate: " << header.sampleRate << std::endl;
	std::cout << "Byte Rate: " << header.byteRate << std::endl;
	std::cout << "Block Align: " << header.blockAlign << std::endl;
	std::cout << "Bits Per Sample: " << header.bitsPerSample << std::endl;

	if (strncmp(header.subChunk2ID, "data", 4) != 0)
	{
		std::cerr << "Data sub-chunk expected" << std::endl;
		return -1;
	}

	uint32_t dataSize = header.subChunk2Size;

	std::cout << "Size of data sub-chunk: " << dataSize << std::endl;

	// Allocate memory for data
	void* data = nullptr;
	data = malloc(dataSize);

	if (data == nullptr)
	{
		std::cerr << "Could not allocate memory" << std::endl;
		return -1;
	}

	//fStream.read(static_cast<char*>(data), dataSize);

	char* dataPtr = static_cast<char*>(data);

	for (uint32_t i{ 0 }; i < dataSize; ++i)
	{
		fStream.read(dataPtr + i, 1);
	}

	for (uint32_t i{ 0 }; i < 11000; ++i)
	{
		std::cout << (int)((char*)data)[i] << " ";
	}

	std::cout << std::endl;

	free(data);
	fStream.close();
	return 0;
}

int print_WAV_content()
{
	WaveFile file{ "Jump3.wav" };

	for (uint32_t i{ 0 }; i < file.getSize(); ++i)
	{
		int val = (int)((char*)file.getData())[i];
		std::cout << std::hex << val << " ";
	}
	return 0;
}

glm::vec3 getVector(int x1, int z1, int x2, int z2, const TGA& tex)
{
	glm::vec3 vec;

	vec.x = x2 - x1;
	vec.z = z2 - z1;

	vec.y = tex.getPixels()[(x2 + z2 * tex.getWidth()) * ((tex.hasAlpha() ? 32 : 24) / 8)] / 10.0 -
		tex.getPixels()[(x1 + z1 * tex.getWidth()) * ((tex.hasAlpha() ? 32 : 24) / 8)] / 10.0;

	return vec;
}

float getGridHeight(int x, int z, Model* model, int width)
{
	return model->vertexArray[(x + width*z) * 3 + 1];;
}

float getHeight(float x, float z, Model* model, int width)
{
	int x_int = static_cast<int>(x);
	float x_rem = x - x_int;

	int z_int = static_cast<int>(z);
	float z_rem = z - z_int;

	if (x_rem + z_rem < 1.f)
	{
		float c = getGridHeight(x_int, z_int, model, width);

		float a = getGridHeight(x_int + 1, z_int, model, width) - c;

		float b = getGridHeight(x_int, z_int + 1, model, width) - c;

		return a*x_rem + b*z_rem + c;
	}

	x_int += 1;
	z_int += 1;

	x_rem = -(1 - x_rem);
	z_rem = -(1 - z_rem);

	float c = getGridHeight(x_int, z_int, model, width);

	float a = c - getGridHeight(x_int - 1, z_int, model, width);

	float b = c - getGridHeight(x_int, z_int - 1, model, width);

	return a*x_rem + b*z_rem + c;
}

Model* GenerateTerrain(const std::string& path, int* width)
{
	TGA textureFile(path.c_str());

	int vertexCount = textureFile.getWidth() * textureFile.getHeight();
	int triangleCount = (textureFile.getWidth() - 1) * (textureFile.getHeight() - 1) * 2;
	int x, z;

	*width = textureFile.getWidth();

	GLfloat *vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount * 3);

	int bpp = textureFile.hasAlpha() ? 32 : 24;

	printf("bpp %d\n", bpp);
	for (x = 0; x < textureFile.getWidth(); x++)
		for (z = 0; z < textureFile.getHeight(); z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * textureFile.getWidth()) * 3 + 0] = x / 1.0;
			vertexArray[(x + z * textureFile.getWidth()) * 3 + 1] = textureFile.getPixels()[(x + z * textureFile.getWidth()) * (bpp / 8)] / 10.0;
			vertexArray[(x + z * textureFile.getWidth()) * 3 + 2] = z / 1.0;

			std::list<int> indexList = { 1,2,3,4,5,6 };

			if (x == 0)
			{
				indexList.remove(1);
				indexList.remove(5);
				indexList.remove(6);
			}
			else if (x == textureFile.getWidth() - 1)
			{
				indexList.remove(2);
				indexList.remove(3);
				indexList.remove(4);
			}

			if (z == 0)
			{
				indexList.remove(1);
				indexList.remove(2);
				indexList.remove(3);
			}
			else if (z == textureFile.getHeight() - 1)
			{
				indexList.remove(4);
				indexList.remove(5);
				indexList.remove(6);
			}

			std::vector<glm::vec3> normals;

			for (auto& it : indexList)
			{
				switch (it)
				{
				case 1:
					normals.push_back(2.f*normalize(cross(getVector(x, z, x, z - 1, textureFile), getVector(x, z, x - 1, z, textureFile))));
					break;
				case 2:
					normals.push_back(normalize(cross(getVector(x, z, x + 1, z - 1, textureFile), getVector(x, z, x, z - 1, textureFile))));
					break;
				case 3:
					normals.push_back(normalize(cross(getVector(x, z, x + 1, z, textureFile), getVector(x, z, x + 1, z - 1, textureFile))));
					break;
				case 4:
					normals.push_back(2.f*normalize(cross(getVector(x, z, x, z + 1, textureFile), getVector(x, z, x + 1, z, textureFile))));
					break;
				case 5:
					normals.push_back(normalize(cross(getVector(x, z, x - 1, z, textureFile), getVector(x, z, x - 1, z + 1, textureFile))));
					break;
				case 6:
					normals.push_back(normalize(cross(getVector(x, z, x - 1, z + 1, textureFile), getVector(x, z, x, z + 1, textureFile))));
					break;
				}
			}

			glm::vec3 normal{};

			for (auto& it : normals)
			{
				normal += it;
			}

			normal = normalize(normal);

			// Normal vectors. You need to calculate these.
			normalArray[(x + z * textureFile.getWidth()) * 3 + 0] = normal.x;
			normalArray[(x + z * textureFile.getWidth()) * 3 + 1] = normal.y;
			normalArray[(x + z * textureFile.getWidth()) * 3 + 2] = normal.z;
			// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * textureFile.getWidth()) * 2 + 0] = x; // (float)x / textureFile.getWidth();
			texCoordArray[(x + z * textureFile.getWidth()) * 2 + 1] = z; // (float)z / textureFile.getHeight();
		}
	for (x = 0; x < textureFile.getWidth() - 1; x++)
		for (z = 0; z < textureFile.getHeight() - 1; z++)
		{
			// Triangle 1
			indexArray[(x + z * (textureFile.getWidth() - 1)) * 6 + 0] = x + z * textureFile.getWidth();
			indexArray[(x + z * (textureFile.getWidth() - 1)) * 6 + 1] = x + (z + 1) * textureFile.getWidth();
			indexArray[(x + z * (textureFile.getWidth() - 1)) * 6 + 2] = x + 1 + z * textureFile.getWidth();
			// Triangle 2
			indexArray[(x + z * (textureFile.getWidth() - 1)) * 6 + 3] = x + 1 + z * textureFile.getWidth();
			indexArray[(x + z * (textureFile.getWidth() - 1)) * 6 + 4] = x + (z + 1) * textureFile.getWidth();
			indexArray[(x + z * (textureFile.getWidth() - 1)) * 6 + 5] = x + 1 + (z + 1) * textureFile.getWidth();
		}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
		vertexArray,
		normalArray,
		texCoordArray,
		NULL,
		indexArray,
		vertexCount,
		triangleCount * 3);

	return model;
}

int labb4_world()
{
	try
	{
		//============================================================================
		// Init
		//============================================================================

		Window window{ 1068, 720, "Test" };

		//============================================================================
		// Vertex buffer setup
		//============================================================================

		int terrainWidth;

		Model* m3;
		m3 = GenerateTerrain("fft-terrain.tga", &terrainWidth);

		Model* sphere;
		sphere = LoadModelPlus("groundsphere.obj");

		Model* skybox;
		skybox = LoadModelPlus("skybox.obj");

		//============================================================================
		// Textures
		//============================================================================

		glActiveTexture(GL_TEXTURE0);

		// TODO: Encapsulate for guarantee of memory release
		TextureMap textures;
		textures.emplace("Concrete", new Texture2D{ "conc.tga" });
		textures.emplace("Flower", new Texture2D{ "maskros512.tga" });
		textures.emplace("ground", new Texture2D{ "grass.tga" });
		textures.emplace("skybox", new Texture2D("SkyBox512.tga", CLAMP_TO_EDGE, CLAMP_TO_EDGE));

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

		ShaderProgram normalShaderProgram{ "normalvert.shader", "normalFrag.shader", "normalgeom.shader" };
		try
		{
			normalShaderProgram.compile();

			normalShaderProgram.bindAttribLocation(0, "vertex_position");
			normalShaderProgram.bindAttribLocation(1, "vertex_normal");

			normalShaderProgram.link();
			normalShaderProgram.use();
		}
		catch (const ShaderProgramException& ex)
		{
			std::cerr << ex.what() << std::endl;
			glfwTerminate();
			return -1;
		}

		ShaderProgram skyboxShaderProgram("skyboxVertex.shader", "skyboxFragment.shader");
		try
		{
			skyboxShaderProgram.use();
			skyboxShaderProgram.compile();

			skyboxShaderProgram.bindAttribLocation(0, "vertex_position");
			skyboxShaderProgram.bindAttribLocation(1, "vertex_normal");
			skyboxShaderProgram.bindAttribLocation(2, "vertex_texture_coordinates");

			skyboxShaderProgram.link();
		}
		catch (const ShaderProgramException& ex)
		{
			std::cerr << ex.what() << std::endl;
			glfwTerminate();
			return -1;
		}


		glfwSetErrorCallback(GLFWError);

		//============================================================================
		// Sound Setup
		//============================================================================

		// Dummy read to flush error code.
		alGetError();

		// Open a device
		ALCdevice* device;
		ALCcontext* context;
		ALenum error;

		device = alcOpenDevice(NULL);

		if (!device)
		{
			std::cout << "No Device" << std::endl;
			return -1;
		}

		context = alcCreateContext(device, NULL);

		alcMakeContextCurrent(context);

		if (!context)
		{
			std::cout << "No Context" << std::endl;
			return -1;
		}

		AudioSource musicSource{ "Music.wav", true };
		AudioSource jumpSource{ "Jump3.wav" };
		AudioListener listener{};

		musicSource.play();

		//============================================================================
		// Final Setup
		//============================================================================

		glClearColor(0.f, 0.f, 0.f, 1.0f);

		GLfloat time = (GLfloat)glfwGetTime();
		GLfloat timeElapsed = 0.f;
		GLuint frames = 0;

		glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		Camera camera{ glm::vec3(0.f,10.f,4.f) };

		bool firstMouse = true;
		GLfloat lastX{ 0.f };
		GLfloat lastY{ 0.f };

		bool forwardKeyPressed{ false };
		bool backwardKeyPressed{ false };
		bool leftKeyPressed{ false };
		bool rightKeyPressed{ false };

		bool upArrowPressed{ false };
		bool downArrowPressed{ false };
		bool leftArrowPressed{ false };
		bool rightArrowPressed{ false };

		bool drawNormals{ false };
		bool wireFrame{ false };

		glm::vec2 sphere_pos{ 1.f,1.f };

		float sphere_speed{ 1.f };

		while (!window.shouldClose())
		{
			// Clear screen and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if(wireFrame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

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
			while (window.pollEvent(ev))
			{
				switch (ev.type)
				{
				case EventType::KEY_EVENT:
					if (ev.key.key == GLFW_KEY_W)
					{
						if (ev.key.action == Action::PRESS)
							forwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							forwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_S)
					{
						if (ev.key.action == Action::PRESS)
							backwardKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							backwardKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_A)
					{
						if (ev.key.action == Action::PRESS)
							leftKeyPressed = true;
						else if (ev.key.action == Action::RELEASE)
							leftKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_D)
					{
						if (ev.key.action == Action::PRESS)
							rightKeyPressed = true;
						else  if (ev.key.action == Action::RELEASE)
							rightKeyPressed = false;
					}
					if (ev.key.key == GLFW_KEY_F5)
					{
						if (ev.key.action == Action::PRESS)
							recompileShaders(shaderProgram);
					}
					if (ev.key.key == GLFW_KEY_F8)
					{
						if (ev.key.action == Action::PRESS)
							drawNormals = !drawNormals;
					}

					if (ev.key.key == GLFW_KEY_F7)
					{
						if (ev.key.action == Action::PRESS)
							wireFrame = !wireFrame;
					}

					if (ev.key.key == GLFW_KEY_UP)
					{
						if (ev.key.action == Action::PRESS)
							upArrowPressed = true;
						else if (ev.key.action == Action::RELEASE)
							upArrowPressed = false;
					}
					if (ev.key.key == GLFW_KEY_DOWN)
					{
						if (ev.key.action == Action::PRESS)
							downArrowPressed = true;
						else if (ev.key.action == Action::RELEASE)
							downArrowPressed = false;
					}
					if (ev.key.key == GLFW_KEY_LEFT)
					{
						if (ev.key.action == Action::PRESS)
							leftArrowPressed = true;
						else if (ev.key.action == Action::RELEASE)
							leftArrowPressed = false;
					}
					if (ev.key.key == GLFW_KEY_RIGHT)
					{
						if (ev.key.action == Action::PRESS)
							rightArrowPressed = true;
						else if (ev.key.action == Action::RELEASE)
							rightArrowPressed = false;
					}
					if (ev.key.key == GLFW_KEY_SPACE)
					{
						if (ev.key.action == Action::PRESS)
							jumpSource.play();
					}
					break;
				case EventType::MOUSE_MOVED_EVENT:
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
					if(window.hasFocus())
						camera.processMouseMovement(xOffset, yOffset);
				}
				break;

				case EventType::GAINED_FOCUS:
				{
					glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				}
				break;
				case EventType::LOST_FOCUS:
				{
					glfwSetInputMode(window.getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

				}
				break;
				default:
					break;
				}
			}

			if (window.hasFocus())
			{
				if (forwardKeyPressed)
					camera.processKeyboard(FORWARD, timeDelta);
				if (backwardKeyPressed)
					camera.processKeyboard(BACKWARD, timeDelta);
				if (leftKeyPressed)
					camera.processKeyboard(LEFT, timeDelta);
				if (rightKeyPressed)
					camera.processKeyboard(RIGHT, timeDelta);

				if (upArrowPressed)
					sphere_pos += glm::vec2{ sphere_speed*timeDelta, 0.f };
				if (downArrowPressed)
					sphere_pos -= glm::vec2{ sphere_speed*timeDelta, 0.f };
				if (leftArrowPressed)
					sphere_pos -= glm::vec2{ 0.f, sphere_speed*timeDelta };
				if (rightArrowPressed)
					sphere_pos += glm::vec2{ 0.f, sphere_speed*timeDelta };

			}

			jumpSource.setPosition(glm::vec3(sphere_pos.x, getHeight(sphere_pos.x, sphere_pos.y, m3, terrainWidth), sphere_pos.y));
			musicSource.setPosition(camera.getPosition());
			listener.setPosition(camera.getPosition());
			listener.setOrientationAt(camera.getFrontVector());
			listener.setOrientationUp(camera.getUpVector());

			// Sätt till 1.f för att aktivera ljud
			listener.setGain(0.f);

			shaderProgram.use();

			glm::mat4 view = camera.getViewMatrix();
			glm::mat4 projection = glm::perspective(radians(45.f), (GLfloat)window.getDimensions().x / (GLfloat)window.getDimensions().y, 0.1f, 1000.f);

			shaderProgram.uploadUniform("time", time);

			shaderProgram.uploadUniform("view_pos", camera.getPosition());

			// Light uniforms

			glm::vec3 lightPos(100.f, 100.f, 100.f);
			//glm::vec3 lightPos = camera.getPosition();
			glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);
			glm::vec3 diffuseColor = lightColor*0.7f; // Decrease the influence
			glm::vec3 ambientColor = diffuseColor*0.3f; // Low influence

			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.position"), lightPos.x, lightPos.y, lightPos.z);

			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.constant"), 1.f);
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.linear"), 0.f);
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.quadratic"), 0.f);

			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "light.specular"), 1.0f, 1.0f, 1.0f);

			// Material Uniforms
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.ambient"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.diffuse"), 1.f, 1.f, 1.f);
			glUniform3f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.specular"), 1.f, 1.f, 1.f);
			glUniform1f(glGetUniformLocation(shaderProgram.getShaderProgramHandle(), "material.shininess"), 0.6f*128.f);

			skyboxShaderProgram.use();
			glm::mat4 skyboxModel = glm::translate(glm::mat4{ 1.f }, camera.getPosition());

			glm::mat4 skyboxView = camera.getViewMatrix();

			skyboxView[0][3] = 0;
			skyboxView[1][3] = 0;
			skyboxView[2][3] = 0;

			mat4 skyboxTransform = projection*skyboxView*skyboxModel;
			skyboxShaderProgram.uploadUniform("transform", skyboxTransform);
			skyboxShaderProgram.uploadUniform("model", skyboxModel);
			skyboxShaderProgram.uploadUniform("texUnit", 0);

			textures.at("skybox")->bind(0);

			glDisable(GL_DEPTH_TEST);

			glCullFace(GL_FRONT);

			DrawModel(skybox, skyboxShaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			glCullFace(GL_BACK);

			glEnable(GL_DEPTH_TEST);

			shaderProgram.use();

			shaderProgram.uploadUniform("texUnit", 0);
			shaderProgram.uploadUniform("texUnit2", 1);
			textures.at("Concrete")->bind(0);

			glm::mat4 model = glm::translate(mat4{ 1.f }, glm::vec3(sphere_pos.x, getHeight(sphere_pos.x, sphere_pos.y, m3, terrainWidth), sphere_pos.y));
			mat4 trans = projection*view*model;

			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(sphere, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			if (drawNormals)
			{
				normalShaderProgram.use();
				normalShaderProgram.uploadUniform("transform", trans);

				glDrawArrays(GL_POINTS, 0, sphere->numVertices);
			}

			textures.at("Concrete")->bind(0);
			textures.at("ground")->bind(1);

			model = glm::scale(mat4{ 1.f }, glm::vec3(1.f, 1.f, 1.f));
			trans = projection*view*model;
			shaderProgram.uploadUniform("transform", trans);
			shaderProgram.uploadUniform("model", model);

			DrawModel(m3, shaderProgram.getShaderProgramHandle(), "vertex_position", "vertex_normal", "vertex_texture_coordinates");

			if (drawNormals)
			{
				normalShaderProgram.use();
				normalShaderProgram.uploadUniform("transform", trans);

				glDrawArrays(GL_POINTS, 0, m3->numVertices);
			}

			window.display();
		}

		musicSource.stop();
		jumpSource.stop();

		DisposeModel(m3);
		DisposeModel(sphere);

		glfwTerminate();

		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);

		error = alGetError();
		if (error != AL_NO_ERROR)
		{
			std::cerr << '12' << alcGetString(device, error) << std::endl;
			return -1;
		}
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

int main()
{
	return labb4_world();
}