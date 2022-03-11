#include "Window.h"
#include "../ErrorHandling/ErrorHandling.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


Window::Window(unsigned int width,unsigned int height)
	:m_Width(width), m_Height(height)
{
	if (!glfwInit())
		std::cout << "Error Initializing GLFW" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	m_Window = glfwCreateWindow(width, height, "BLOCK BREAKER", NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		std::cout << "Error Creating Window" << std::endl;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR while initializing glew\n";

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugMessageCallback, NULL);

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;

	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

	glViewport(0, 0, m_Width, m_Height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}