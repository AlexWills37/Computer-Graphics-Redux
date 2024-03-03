#pragma once
#include "GLFW/glfw3.h"
#include <unordered_map>
#include "glm/glm.hpp"

enum Input {
	NONE, 
	UP, DOWN, LEFT, RIGHT,
	W, A, S, D,
	Q, E,
	SHIFT, CONTROL,
	LAST
};

class InputModule {
public:
	void operator=(const InputModule&) = delete;
	InputModule(InputModule& other) = delete;

	bool QueryInput(Input toCheck);
	inline bool IsLeftMousePressed() { return m_IsClicking; }
	inline glm::dvec2 GetCurosrVelocity() { return glm::dvec2(m_CursorVelocity[0], m_CursorVelocity[1]); }


	static InputModule* GetInstance(GLFWwindow* window);
	static InputModule* InitializeInputModule(GLFWwindow* window);

protected:
	std::unordered_map<Input, bool> inputStates;
	bool m_IsClicking = false;
	double m_PreviousPos[2] = { 0, 0 };
	double m_CursorVelocity[2] = { 0, 0 };

protected:
	InputModule(GLFWwindow* window);
	static InputModule* singleton;

	void InstancedKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void InstancedCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void InstancedMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};