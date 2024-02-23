#include "Display.h"
#include <iostream>



Display::Display()
    
{
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "ERROR! Could not initialize GLFW library." << std::endl;
    }

    // Use OpenGL 3.3 with the CORE profile
    //  CORE - multiple VAOs
    //  COMPATABILITY (COMPAT) - single global default VAO
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode m_Window and its OpenGL context */
    m_Window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        std::cout << "ERROR! Could not initialize GLFW window." << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);    // Synchronize frame updates with V sync

}


Display::~Display()
{
    glfwTerminate();
}

bool Display::WindowShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Display::EndFrame()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);

    /* Poll for and process events */
    glfwPollEvents();
}
