#include "Window.h"

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

	settings.samples = 4;
	settings.vSync = GLFW_TRUE;

	settings.version_major = 3;
	settings.version_minor = 3;

	return settings;
}

Window::Window(const GLuint width, const GLuint height, const std::string& title, WindowSettings settings)
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

	glViewport(0, 0, width, height);

	if(settings.vSync)
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
}

Window::~Window()
{
	glfwDestroyWindow(windowHandle);
	glfwTerminate();
}

glm::vec2 Window::getDimensions() const noexcept
{
	int width, height;
	glfwGetWindowSize(windowHandle, &width, &height);
	return glm::vec2{ width, height };
}

GLFWwindow* Window::getHandle() const noexcept
{
	return windowHandle;
}

void Window::setDimensions(const GLuint newWidth, const GLuint newHeight) const
{
	setContextCurrent();
	glViewport(0, 0, newWidth, newHeight);
}

void Window::setDimensions(const glm::vec2& newDimensions) const
{
	setContextCurrent();
	glViewport(0, 0, newDimensions.x, newDimensions.y);
}

void Window::setTitle(const std::string& newTitle) const
{
	glfwSetWindowTitle(windowHandle, newTitle.c_str());
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
	setContextCurrent();
	glViewport(0, 0, width, height);
}

void Window::key_callback(int key, int scancode, int action, int mods)
{
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
