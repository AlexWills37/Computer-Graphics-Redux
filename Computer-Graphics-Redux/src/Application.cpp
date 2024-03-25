#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Display.h"
#include "Shader.h"
#include "Transform.h"
#include "Model.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Input.h"
#include "Camera.h"

#include "demos/SandboxDemo.h"
#include "demos/NormalSphereDemo.h"
#include "demos/GouraudDemo.h"
#include "demos/PhongDemo.h"


int main() {
	Display window;
	Display::SetDisplayPointer(&window);
	InputModule* inputMod = InputModule::InitializeInputModule(window.GetWindow());


	/* Initialize GLEW */
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;

	demo::Demo* d = new demo::PhongDemo();

	while (!window.WindowShouldClose())
	{
		d->OnUpdate(0.0f);
		d->OnRender();
		window.EndFrame();
	}

	return 0;
}
