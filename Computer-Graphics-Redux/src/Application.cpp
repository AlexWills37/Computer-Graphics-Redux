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



int main() {


	Camera c = Camera(960, 500, 90, 0.1f, 100.0f);
	/*c.Translate(glm::vec3(-1, -2, 5));
	std::cout << "0 pitch 0 yaw" << std::endl;
	c.GetInverseCameraMatrix();

	std::cout << "\n90 pitch 0 yaw" << std::endl;
	c.Rotate(0, 3.14 / 2);
	c.GetInverseCameraMatrix();

	std::cout << "\n180 pitch 0 yaw" << std::endl;
	c.Rotate(0, 3.14 / 2);
	c.GetInverseCameraMatrix();


	std::cout << "\n90 pitch 90 yaw" << std::endl;
	c.Rotate(3.14/2, -3.14 / 2);
	c.GetInverseCameraMatrix();

	std::cout << "\n90 pitch 180 yaw" << std::endl;
	c.Rotate(3.14 / 2, 0);
	c.GetInverseCameraMatrix();
	*/

	c.Rotate(0, 3.14 / 2);
	


    Display window;


    /* Initialize GLEW */
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    std::cout << "Using OpenGL: " << glGetString(GL_VERSION) << std::endl;


 
    Shader shader = Shader("res/shaders/Basic");

    GLuint vao, ib, vb;
	

	/*std::vector<Vertex> vertices = {
		// Back face
		{-0.5f, -0.5f, -0.5f, 0.4f, 0.8f, 0.9f},	// back left bottom
		{-0.5f,  0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f,  0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f, -0.5f, -0.5f, 0.4f, 0.8f, 0.9f},


		// Top face
		{-0.5f,  0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{-0.5f,  0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f,  0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f,  0.5f, -0.5f, 0.4f, 0.8f, 0.9f},

		// Front face
		{-0.5f,  0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{-0.5f, -0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f, -0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f,  0.5f,  0.5f, 0.4f, 0.8f, 0.9f},

		// Bottom face
		{-0.5f, -0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{-0.5f, -0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f, -0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f, -0.5f,  0.5f, 0.4f, 0.8f, 0.9f},

		// Right face
		{ 0.5f, -0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f, -0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f,  0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{ 0.5f,  0.5f,  0.5f, 0.4f, 0.8f, 0.9f},

		// Left face
		{-0.5f,  0.5f,  0.5f, 0.4f, 0.8f, 0.9f},
		{-0.5f,  0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{-0.5f, -0.5f, -0.5f, 0.4f, 0.8f, 0.9f},
		{-0.5f, -0.5f,  0.5f, 0.4f, 0.8f, 0.9f}
	};

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	}; */


	int indexCount = 0;
	{
		Model cube = Model::CreateSphere(25);
		//Model cube = Model::CreateCube();
		std::vector<uint32_t> indices = cube.GetIndices();
		indexCount = indices.size();
		std::vector<Vertex> vertices = cube.GetVertices();


		VertexBufferLayout layout = cube.GetLayout();

		// Create vao
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Create vb
		glGenBuffers(1, &vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


		const auto& attributes = layout.GetAttributes();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < attributes.size(); i++) {
			const auto& attribute = attributes[i];

			// Now use the layout to make attribute pointers to store in the current VAO
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized,
				sizeof(Vertex), (const void*)offset);

			offset += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
		}

		// Create index buffer
		glGenBuffers(1, &ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
		shader.Bind();
	}

	int window_width = 960;
	int window_height = 540;

	Transform model = Transform(glm::vec3(3, 0, -7	), glm::vec3(0, 0, 0), glm::vec3(5, 5, 5));
	//glm::mat4 projection = glm::perspective(glm::radians(90.0f), window_width / (float)window_height, 1.0f, 100.0f);
	
	
	glm::mat4 mvp;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glDepthFunc(GL_ALWAYS);

	
	// create input module
	InputModule* inputMod = InputModule::InitializeInputModule(window.GetWindow());
	float moveSpeed = 0.3f;
	float rotSpeed = 1;

	Camera cam = Camera(960, 540, 90, 0.1f, 100.0f);

    /* Loop until the user closes the window */
    while (!window.WindowShouldClose())
    {
		// Move camera
		glm::vec3 change = glm::vec3(0);
		glm::vec2 rotation = glm::vec2(0);

		if (inputMod->QueryInput(Input::S))
			change.z += moveSpeed;
		if (inputMod->QueryInput(Input::W))
			change.z -= moveSpeed;
		if (inputMod->QueryInput(Input::A))
			change.x -= moveSpeed;
		if (inputMod->QueryInput(Input::D))
			change.x += moveSpeed;
		if (inputMod->QueryInput(Input::Q))
			change.y -= moveSpeed;
		if (inputMod->QueryInput(Input::E))
			change.y += moveSpeed;
		if (inputMod->QueryInput(Input::LEFT))
			rotation.x -= rotSpeed;
		if (inputMod->QueryInput(Input::RIGHT))
			rotation.x += rotSpeed;
		if (inputMod->QueryInput(Input::UP))
			rotation.y -= rotSpeed;
		if (inputMod->QueryInput(Input::DOWN))
			rotation.y += rotSpeed;

		rotation *= 3.14f / 180;
		cam.Rotate(rotation.x, rotation.y);
		cam.Move(change);



		mvp = cam.GetWorldToScreenMatrix() * model.GetMatrix();


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //renderer.RenderScene(scene);

		//mvp = projectionMatrix * camera.GetMatrix() * Transform(m_Transform, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)).GetMatrix();
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

		


        window.EndFrame();
    }



	return 0;
}