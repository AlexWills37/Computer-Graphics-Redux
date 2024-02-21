#pragma once
#include <GLFW/glfw3.h>

class Display {
private:
	GLFWwindow* m_Window;

public:
	Display();
	~Display();

	bool WindowShouldClose();
	void EndFrame();

	inline GLFWwindow* GetWindow() { return m_Window; }
};