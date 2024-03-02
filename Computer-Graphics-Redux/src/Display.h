#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


class Display {
private:
	GLFWwindow* m_Window;

public:
	Display();
	~Display();

	bool WindowShouldClose();
	void EndFrame();

	inline GLFWwindow* GetWindow() { return m_Window; }
	
	static void SetDisplayPointer(Display* displayPtr) { s_DisplayPtr = displayPtr; }
	static Display* GetDisplayPointer();

private:

	static Display* s_DisplayPtr;
};