#pragma once
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

class Window
{
public:
	Window(unsigned int width, unsigned int height);
	~Window();

	void Clear();
	void Update();

	inline bool isClosed() { return glfwWindowShouldClose(m_Window); }
	
	inline GLFWwindow* GetWindow() { return m_Window; }

private:
	GLFWwindow* m_Window;
	unsigned int m_Width, m_Height;
};

