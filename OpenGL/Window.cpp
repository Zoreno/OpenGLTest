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

Window::Window(const GLuint width, const GLuint height, const std::string& title)
{
	if (!glfwInit())
	{
		throw WindowRuntimeException("Failed to initialize GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	// Disable vsync
	glfwSwapInterval(0);

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
	glViewport(0, 0, newWidth, newHeight);
}

void Window::setDimensions(const glm::vec2& newDimensions) const
{
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
