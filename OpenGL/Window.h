/**
 * @file	Window.h
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	Window related structs and classes.
 */

#pragma once

#include <stdexcept>
#include <queue>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

 /**
  * @brief Exception thrown when runtime errors occur.
  */
class WindowRuntimeException : std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

/**
* @brief Exception thrown when logic errors occur.
*/
class WindowLogicException : std::logic_error
{
public:
	using std::logic_error::logic_error;
};

/**
 * @brief Enum containing all event type codes.
 */
enum EventType
{
	/**
	 * @brief Type code for keypress/release event
	 */
	KEY_EVENT = 0,

	/**
	 * @brief Type code for mouse event
	 */
	MOUSE_EVENT,

	/**
	 * @brief Total number of used event codes
	 */
	NUM_EVENTS
};

/**
 * @brief Struct containing data representing an event.
 */
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

/**
 * @brief Struct containing customizable window settings.
 */
struct WindowSettings
{
	//============================================================================
	// Window Related Settings
	//============================================================================

	/**
	 * @brief Window is resizable
	 */
	GLboolean resizable;

	/**
	 * @brief Window is visible at startup
	 */
	GLboolean visible;

	/**
	 * @brief Window will include borders and window bar
	 */
	GLboolean decorated;

	/**
	 * @brief Window will have focus at startup
	 */
	GLboolean focused;

	/**
	 * @brief Window will automatically be iconified if focus is lost in fullscreen
	 */
	GLboolean auto_iconify;

	/**
	 * @brief Window will always be on top
	 */
	GLboolean floating;

	/**
	 * @brief Window will initially be maximized
	 */
	GLboolean maximized;

	/**
	 * @brief Minimum width of window.
	 */
	GLuint minXLimit;

	/**
	 * @brief Maximum width of window.
	 */
	GLuint maxXLimit;
	/**
	 * @brief Minimum height of window.
	 */
	GLuint minYLimit;

	/**
	 * @brief Maximum height of window.
	 */
	GLuint maxYLimit;

	//============================================================================
	// Framebuffer Related Settings
	//============================================================================

	/**
	 * @brief How many samples should multisampling use.
	 */
	GLuint samples;

	/**
	 * @brief Sync redraw rate with screen redraw rate.
	 */
	GLboolean vSync;

	//============================================================================
	// Context Related Settings
	//============================================================================

	/**
	 * @brief Major number of desired version
	 */
	GLuint version_major;

	/**
	 * @brief Minor number of desired verison
	 */
	GLuint version_minor;
};

/**
 * @brief Default Window Settings
 *
 * Constructs a struct containing the default values of the window settings.
 *
 * @return Struct containing default settings.
 */
WindowSettings getDefaultWindowSettings();

/**
 * @brief Representation of a window
 */
class Window
{
public:

	/**
	 * @brief Default Constructor
	 * @note  Deleted
	 */
	Window() = delete;

	/**
	* @brief Copy Constructor
	* @note  Deleted
	*/
	Window(const Window&) = delete;

	/**
	* @brief Move Constructor
	* @note  Deleted
	*/
	Window(Window&&) = delete;

	/**
	 * @brief Constructor for the window
	 *
	 * Creates a new window from the given parameters. Initializes GLFW and GLEW the first time a window is created.
	 *
	 * @param width Width of the window
	 * @param height Height of the window
	 * @param title Title of the window
	 * @param settings Desired settings.
	 */
	Window(const GLuint width, const GLuint height, const std::string& title, WindowSettings settings = getDefaultWindowSettings());


	/**
	 * @brief Destructor for the window
	 *
	 * Removes and cleans up after the window.
	 * If this is the last window, GLFW will be terminated.
	 */
	~Window();

	/**
	 * @brief Copy assignment operator
	 * @note  Deleted
	 */
	const Window& operator=(const Window&) = delete;

	/**
	* @brief Move assignment operator
	* @note  Deleted
	*/
	Window& operator=(Window&&) = delete;

	/**
	 * @brief Get window dimentions
	 * @return Width and Height packed into a vector
	 */
	glm::uvec2 getDimensions() const noexcept;

	/**
	 * @brief Get Window Width
	 * @return Width of the Window
	 */
	GLuint getWidth() const noexcept;

	/**
	 * @brief Get Window Height
	 * @return Height of the Window.
	 */
	GLuint getHeight() const noexcept;

	/**
	 * @brief Get Window Handle
	 * @return The GLFW window handle
	 */
	GLFWwindow* getHandle() const noexcept;

	/**
	 * @brief Sets New Dimensions
	 * @param newWidth New Width of the Window
	 * @param newHeight New Height of the Window
	 */
	void setDimensions(const GLuint newWidth, const GLuint newHeight);

	/**
	 * @brief Sets new Dimensions
	 * @param newDimensions New Dimensions of the Window
	 */
	void setDimensions(const glm::uvec2& newDimensions);

	/**
	 * @brief Get X Position
	 * @return X-position
	 */
	GLuint getXPos() const noexcept;
	
	/**
	 * @brief Get Y Position 
	 * @return Y-position
	 */
	GLuint getYPos() const noexcept;

	/**
	 * @brief Get Position of window.
	 * @return Position packed as a vector
	 */
	glm::uvec2 getPosition() const noexcept;

	/**
	 * @brief Sets new Title
	 * @param newTitle New Title of the Window
	 */
	void setTitle(const std::string& newTitle);

	/**
	 * @brief Gets the current title
	 * @return Current window title
	 */
	std::string getTitle() const noexcept;

	/**
	 * @brief Window Should Close flag
	 * @return True if window should close.
	 */
	bool shouldClose() const;

	/**
	 * @brief Sets the Window Should Close Flag
	 */
	void requestClose() const;

	/**
	 * @brief Displays the content of the framebuffer in the window.
	 *
	 * This is done by swapping buffers. Also polls events related to this window.
	 */
	void display() const;

	/**
	 * @brief Sets this window as the current context.
	 */
	void setContextCurrent() const;

	/**
	 * @brief Polls for any window events
	 * @param ev Reference to struct where data will be stored.
	 * @return False if there is no more events to handle.
	 */
	bool pollEvent(WindowEvent& ev);

	/**
	 * @brief Enables vertical sync for this window.
	 */
	void enableVSync();

	/**
	 * @brief Disables vertical sync for this window
	 */
	void disableVSync();

	/**
	 * @brief Gets the vertical sync status.
	 * @return True if vertical sync is enabled.
	 */
	bool getVSyncStatus() const;

	/**
	 * @brief Minimized the window.
	 */
	void iconify() const;

	/**
	 * @brief Restores the window from a minimized state.
	 */
	void restore() const;

private:

	/**
	 * @brief Framebuffer size callback handler
	 * @param windowHandle Handle to the window being resized
	 * @param width New Width of window
	 * @param height New Height of window
	 */
	friend void framebuffer_size_callback_func(GLFWwindow* windowHandle, int width, int height);

	/**
	 * @brief Keyboard callback handler
	 * @param windowHandle Handle to the window getting the key press/release
	 * @param key Virtual keycode for the key
	 * @param scancode Scancode for the key
	 * @param action Action performed (press/release)
	 * @param mods Modifier keys pressed at the same time
	 */
	friend void key_callback_func(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);

	/**
	 * @brief Mouse callback handler
	 * @param windowHandle Handle to the window getting the mouse update
	 * @param xpos New x-position of the mouse
	 * @param ypos New y-position of the mouse
	 */
	friend void mouse_callback_func(GLFWwindow* windowHandle, double xpos, double ypos);

	/**
	 * @brief Window position callback handler
	 * @param windowHandle Handle to the window being moved.
	 * @param xpos New x-position of the window.
	 * @param ypos New y-position of the window.
	 */
	friend void window_pos_callback_func(GLFWwindow* windowHandle, int xpos, int ypos);

	/**
	 * @brief Class member callback for framebuffer change
	 * @param width New Width of window.
	 * @param height New Height of window.
	 */
	void framebuffer_size_callback(int width, int height);

	/**
	 * @brief Class member callback for key press/release
	 * @param key Virtual keycode for the key
	 * @param scancode Scancode for the key
	 * @param action Action performed (press/release)
	 * @param mods Modifier keys pressed at the same time
	 */
	void key_callback(int key, int scancode, int action, int mods);

	/**
	 * @brief Class member callback for mouse movement
	 * @param xpos New x-position of the mouse
	 * @param ypos New y-position of the mouse
	 */
	void mouse_callback(double xpos, double ypos);

	/**
	 * @brief Class member callback for
	 * @param xpos New x-position of the window.
	 * @param ypos New y-position of the window.
	 */
	void window_pos_callback(int xpos, int ypos);

	/**
	 * @brief GLFW Window Handle
	 */
	GLFWwindow* windowHandle;

	/**
	 * @brief Window Event Queue
	 */
	std::queue<WindowEvent> eventQueue;

	/**
	 * @brief VSync status
	 */
	bool VSyncActive{ false };

	/**
	 * @brief Number of currently instantiated windows
	 */
	static int INSTANCE_COUNT;

	/**
	 * @brief Width of the Window.
	 */
	GLuint width;

	/**
	 * @brief Height of the window.
	 */
	GLuint height;

	/**
	 * @brief X-position of the window relative to the screen.
	 */
	GLuint xpos;

	/**
	* @brief Y-position of the window relative to the screen.
	*/
	GLuint ypos;

	/**
	 * @brief The title of the window.
	 */
	std::string title;
};