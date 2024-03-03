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
			toChange = S;
			break;
		case GLFW_KEY_W:
			toChange = W;
			break;
		case GLFW_KEY_A:
			toChange = A;
			break;
		case GLFW_KEY_D:
			toChange = D;
			break;
		case GLFW_KEY_Q:
			toChange = Q;
			break;
		case GLFW_KEY_E:
			toChange = E;
			break;
		case GLFW_KEY_LEFT:
			toChange = LEFT;
			break;
		case GLFW_KEY_RIGHT:
			toChange = RIGHT;
			break;
		case GLFW_KEY_UP:
			toChange = UP;
			break;
		case GLFW_KEY_DOWN:
			toChange = DOWN;
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			toChange = SHIFT;
			break;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL:
			toChange = CONTROL;
			break;
		}

		if (toChange != NONE) {
			inputStates[toChange] = action == GLFW_PRESS;
		}
	}
}

void InputModule::InstancedCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	m_CursorVelocity[0] = xpos - m_PreviousPos[0];
	m_CursorVelocity[1] = ypos - m_PreviousPos[1];
	std::cout << m_CursorVelocity[0] << ", " << m_CursorVelocity[1] << std::endl;
	m_PreviousPos[0] = xpos;
	m_PreviousPos[1] = ypos;
}

void InputModule::InstancedMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1) {
			m_IsClicking = (action == GLFW_PRESS);
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

void InputModule::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Add the callback to the singleton instance
	if (singleton != nullptr) {
		GetInstance(window)->InstancedCursorPosCallback(window, xpos, ypos);
	}
	else {
		std::cout << "Error! Input Module not initialized!" << std::endl;
	}
}

void InputModule::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Add the callback to the singleton instance
	if (singleton != nullptr) {
		GetInstance(window)->InstancedMouseButtonCallback(window, button, action, mods);
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
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetCursorPosCallback(window, cursor_position_callback);

	return mod;
}
