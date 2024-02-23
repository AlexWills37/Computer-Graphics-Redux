#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


static glm::vec3 m_Transform = glm::vec3(0, 0, 0);
class Display {
private:
	GLFWwindow* m_Window;
	//glm::vec3 m_Transform;

public:
	Display();
	~Display();

	bool WindowShouldClose();
	void EndFrame();

	inline GLFWwindow* GetWindow() { return m_Window; }
	inline glm::vec3 GetTransform() { return m_Transform; }

private:
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};