#pragma once
#include "GLFW/glfw3.h"
#include <unordered_map>

enum Input {
	NONE, 
	UP, DOWN, LEFT, RIGHT,
	W, A, S, D,
	Q, E,
	LAST
};

class InputModule {
protected:
	std::unordered_map<Input, bool> inputStates;

protected:
	InputModule(GLFWwindow* window);
	static InputModule* singleton;

	void InstancedKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


public:
	void operator=(const InputModule&) = delete;
	InputModule(InputModule& other) = delete;

	bool QueryInput(Input toCheck);
	static InputModule* GetInstance(GLFWwindow* window);
	static InputModule* InitializeInputModule(GLFWwindow* window);
};