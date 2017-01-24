#pragma once

#include <iostream>
#include <stdexcept>
#include <functional>
#include <queue>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class WindowRuntimeException : std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

class WindowLogicException : std::logic_error
{
public:
	using std::logic_error::logic_error;
};

enum EventType
{
	KEY_EVENT,
	MOUSE_EVENT,

	NUM_EVENTS
};

struct WindowEvent
{
	EventType type;

	union
	{
		struct
		{
			int key;
			int scancode;
			int action;
			int mods;
		} key;

		struct
		{
			double posx;
			double posy;
		} mouse;
	};
};

class Window
{
public:

	Window() = delete;
	Window(const GLuint width, const GLuint height, const std::string& title);
	~Window();
	
	// Non-movable and non-copyable for now.
	const Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	glm::vec2 getDimensions() const noexcept;
	GLFWwindow* getHandle() const noexcept;

	void setDimensions(const GLuint newWidth, const GLuint newHeight) const;
	void setDimensions(const glm::vec2& newDimensions) const;
	void setTitle(const std::string& newTitle) const;

	bool shouldClose() const;
	void requestClose() const;

	void display() const;

	void setContextCurrent() const;

	bool pollEvent(WindowEvent& ev);

private:
	friend void framebuffer_size_callback_func(GLFWwindow* windowHandle, int width, int height);
	friend void key_callback_func(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);
	friend void mouse_callback_func(GLFWwindow* window, double xpos, double ypos);

	void framebuffer_size_callback(int width, int height);
	void key_callback(int key, int scancode, int action, int mods);
	void mouse_callback(double xpos, double ypos);

	GLFWwindow* windowHandle;

	std::queue<WindowEvent> eventQueue;
};