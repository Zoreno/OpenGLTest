/**
 * @file	Window.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	Window Class implementation
 */

#include "Window.h"

int Window::INSTANCE_COUNT = 0;

void framebuffer_size_callback_func(GLFWwindow* windowHandle, int width, int height)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->framebuffer_size_callback(width, height);
}

void key_callback_func(GLFWwindow* windowHandle, int key, int scancode, int action, int mods)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->key_callback(key, scancode, action, mods);
}

void mouse_callback_func(GLFWwindow* windowHandle, double xpos, double ypos)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->mouse_callback(xpos, ypos);
}

void window_pos_callback_func(GLFWwindow* windowHandle, int xpos, int ypos)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->window_pos_callback(xpos, ypos);
}

WindowSettings getDefaultWindowSettings()
{
	WindowSettings settings;

	settings.resizable = GLFW_TRUE;
	settings.visible = GLFW_TRUE;
	settings.decorated = GLFW_TRUE;
	settings.focused = GLFW_TRUE;
	settings.auto_iconify = GLFW_TRUE;
	settings.floating = GLFW_FALSE;
	settings.maximized = GLFW_FALSE;

	settings.minXLimit = GLFW_DONT_CARE;
	settings.maxXLimit = GLFW_DONT_CARE;
	settings.minYLimit = GLFW_DONT_CARE;
	settings.maxYLimit = GLFW_DONT_CARE;

	settings.samples = 4;
	settings.vSync = GLFW_TRUE;

	settings.version_major = 3;
	settings.version_minor = 3;

	return settings;
}

Window::Window(const GLuint width, const GLuint height, const std::string& title, WindowSettings settings)
	: width{ width }, height{ height }, title{title}
{
	// If this is the first window, we intitialize GLFW and GLEW aswell
	if (INSTANCE_COUNT++ == 0)
	{
		if (!glfwInit())
		{
			throw WindowRuntimeException("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
		glfwWindowHint(GLFW_VISIBLE, settings.visible);
		glfwWindowHint(GLFW_DECORATED, settings.decorated);
		glfwWindowHint(GLFW_FOCUSED, settings.focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY, settings.auto_iconify);
		glfwWindowHint(GLFW_FLOATING, settings.floating);
		glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);

		glfwWindowHint(GLFW_SAMPLES, settings.samples); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version_major); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version_minor);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

		windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (windowHandle == nullptr) {
			glfwTerminate();
			throw WindowRuntimeException("Failed to open GLFW window");
		}

		glfwMakeContextCurrent(windowHandle); // Initialize GLEW
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			throw WindowRuntimeException("Failed to initialize GLEW!");
		}

	}
	else // We just need to create a new window
	{
		glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
		glfwWindowHint(GLFW_VISIBLE, settings.visible);
		glfwWindowHint(GLFW_DECORATED, settings.decorated);
		glfwWindowHint(GLFW_FOCUSED, settings.focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY, settings.auto_iconify);
		glfwWindowHint(GLFW_FLOATING, settings.floating);
		glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);

		glfwWindowHint(GLFW_SAMPLES, settings.samples); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version_major); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version_minor);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

		windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (windowHandle == nullptr) {
			glfwTerminate();
			throw WindowRuntimeException("Failed to open GLFW window");
		}

		glfwMakeContextCurrent(windowHandle); // Initialize GLEW
	}

	glViewport(0, 0, width, height);

	// Get the current position
	int window_pos_x, window_pos_y;
	glfwGetWindowPos(windowHandle, &window_pos_x, &window_pos_y);
	xpos = static_cast<GLuint>(window_pos_x);
	ypos = static_cast<GLuint>(window_pos_y);

	glfwSetWindowSizeLimits(
		windowHandle, 
		settings.minXLimit, 
		settings.minYLimit, 
		settings.maxXLimit, 
		settings.maxYLimit);

	if (settings.vSync)
	{
		enableVSync();
	}
	else
	{
		disableVSync();
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(windowHandle, GLFW_STICKY_KEYS, GL_TRUE);

	// Specify that this object will handle window
	glfwSetWindowUserPointer(windowHandle, this);

	// Setup callback functions
	glfwSetFramebufferSizeCallback(windowHandle, framebuffer_size_callback_func);
	glfwSetKeyCallback(windowHandle, key_callback_func);
	glfwSetCursorPosCallback(windowHandle, mouse_callback_func);
	glfwSetWindowPosCallback(windowHandle, window_pos_callback_func);
}

Window::~Window()
{
	--INSTANCE_COUNT;
	glfwDestroyWindow(windowHandle);
	if (INSTANCE_COUNT == 0) // If this was the last window, terminate GLFW
	{
		glfwTerminate();
	}
}

glm::uvec2 Window::getDimensions() const noexcept
{
	int width, height;
	glfwGetWindowSize(windowHandle, &width, &height);
	return glm::vec2{ width, height };
}

GLuint Window::getWidth() const noexcept
{
	return width;
}

GLuint Window::getHeight() const noexcept
{
	return height;
}

GLFWwindow* Window::getHandle() const noexcept
{
	return windowHandle;
}

void Window::setDimensions(const GLuint newWidth, const GLuint newHeight)
{
	width = newWidth;
	height = newHeight;
	setContextCurrent();
	glViewport(0, 0, newWidth, newHeight);
}

void Window::setDimensions(const glm::uvec2& newDimensions)
{
	width = newDimensions.x;
	height = newDimensions.y;
	setContextCurrent();
	glViewport(0, 0, newDimensions.x, newDimensions.y);
}

GLuint Window::getXPos() const noexcept
{
	return xpos;
}

GLuint Window::getYPos() const noexcept
{
	return ypos;
}

glm::uvec2 Window::getPosition() const noexcept
{
	return glm::uvec2(xpos, ypos);
}

void Window::setTitle(const std::string& newTitle)
{
	title = newTitle;
	glfwSetWindowTitle(windowHandle, newTitle.c_str());
}

std::string Window::getTitle() const noexcept
{
	return title;
}

bool Window::shouldClose() const
{
	return (glfwWindowShouldClose(windowHandle) != 0);
}

void Window::requestClose() const
{
	glfwSetWindowShouldClose(windowHandle, GLFW_TRUE);
}

void Window::framebuffer_size_callback(int width, int height)
{
	this->width = width;
	this->height = height;
	setContextCurrent();
	glViewport(0, 0, width, height);
}

void Window::key_callback(int key, int scancode, int action, int mods)
{
	// TODO: Remove and let client handle exit commands
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		requestClose();

	WindowEvent ev;
	ev.type = EventType::KEY_EVENT;
	ev.key.key = key;
	ev.key.scancode = scancode;
	ev.key.action = action;
	ev.key.mods = mods;
	eventQueue.push(ev);
}

void Window::mouse_callback(double xpos, double ypos)
{
	WindowEvent ev;
	ev.type = EventType::MOUSE_EVENT;
	ev.mouse.posx = xpos;
	ev.mouse.posy = ypos;
	eventQueue.push(ev);
}

void Window::window_pos_callback(int xpos, int ypos)
{
	this->xpos = xpos;
	this->ypos = ypos;
}

void Window::display() const
{
	glfwSwapBuffers(windowHandle);
	glfwPollEvents();
}

void Window::setContextCurrent() const
{
	glfwMakeContextCurrent(windowHandle);
}

bool Window::pollEvent(WindowEvent& ev)
{
	if (eventQueue.empty())
		return false;

	ev = eventQueue.front();
	eventQueue.pop();
	return true;
}

void Window::enableVSync()
{
	glfwSwapInterval(1);
	VSyncActive = true;
}

void Window::disableVSync()
{
	glfwSwapInterval(0);
	VSyncActive = false;
}

bool Window::getVSyncStatus() const
{
	return VSyncActive;
}

void Window::iconify() const
{
	glfwIconifyWindow(windowHandle);
}

void Window::restore() const
{
	glfwRestoreWindow(windowHandle);
}
