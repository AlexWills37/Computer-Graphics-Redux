#include "Input.h"
#include <iostream>

InputModule* InputModule::singleton = nullptr;

InputModule::InputModule(GLFWwindow* window)
{
	// Initialize input states list
	for (int i = (int)Input::NONE; i < (int)Input::LAST; i++) {
		inputStates[(Input)i] = false;
	}

}

void InputModule::InstancedKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input toChange = NONE;
	if (action != GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_S:
			toChange = DOWN;
			break;
		case GLFW_KEY_W:
			toChange = UP;
			break;
		case GLFW_KEY_A:
			toChange = LEFT;
			break;
		case GLFW_KEY_D:
			toChange = RIGHT;
			break;
		case GLFW_KEY_Q:
			toChange = Q;
			break;
		case GLFW_KEY_E:
			toChange = E;
			break;
		case GLFW_KEY_LEFT:
			toChange = LEFTA;
			break;
		case GLFW_KEY_RIGHT:
			toChange = RIGHTA;
			break;
		}

		if (toChange != NONE) {
			inputStates[toChange] = action == GLFW_PRESS;
		}
	}
}

void InputModule::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Add the callback to the singleton instance
	if (singleton != nullptr) {
		GetInstance(window)->InstancedKeyCallback(window, key, scancode, action, mods);
	}
	else {
		std::cout << "Error! Input Module not initialized!" << std::endl;
	}
}

bool InputModule::QueryInput(Input toCheck)
{
	if (inputStates.find(toCheck) != inputStates.end()) {
		return inputStates[toCheck];
	}
	return false;
}

InputModule* InputModule::GetInstance(GLFWwindow* window)
{

	if (singleton == nullptr) {
		singleton = new InputModule(window);
	}
	return singleton;
}

InputModule* InputModule::InitializeInputModule(GLFWwindow* window)
{
	// Create the singleton module
	InputModule* mod = GetInstance(window);

	// Connect it to the GLFW system
	glfwSetKeyCallback(window, KeyCallback);

	return mod;
}
