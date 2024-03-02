#include "SandboxDemo.h"

#include "Display.h"
#include "Model.h"

#include "Transform.h"

#include <iostream>

demo::SandboxDemo::SandboxDemo()
{
	// Get input module 
	Display* display = Display::GetDisplayPointer();
	m_InputMod = InputModule::GetInstance(display->GetWindow());

	// Create camera
	m_Camera = std::make_unique<Camera>(960, 500, 90, 0.1f, 100.0f);

	// Create Sphere data
	Model sphere = Model::CreateSphere(25);
	std::vector<uint32_t> indices = sphere.GetIndices();
	m_IndexCount = indices.size();
	std::vector<Vertex> vertices = sphere.GetVertices();

	VertexBufferLayout layout = sphere.GetLayout();

	// Create vao
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create vb
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
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
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	// Create Shader
	m_Shader = std::make_unique<Shader>("res/shaders/Basic");

	// Configue OpenGL things
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Move camera a little bit
	m_Camera->Translate(glm::vec3(0, 0, 5.0f));
}

demo::SandboxDemo::~SandboxDemo()
{
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteVertexArrays(1, &m_VAO);
}

void demo::SandboxDemo::OnUpdate(float deltaTime)
{
	glm::vec3 change = glm::vec3(0);
	glm::vec2 rotation = glm::vec2(0);
	
	if (m_InputMod->QueryInput(Input::S))
		change.z += 1;
	if (m_InputMod->QueryInput(Input::W))
		change.z -= 1;
	if (m_InputMod->QueryInput(Input::A))
		change.x -= 1;
	if (m_InputMod->QueryInput(Input::D))
		change.x += 1;
	if (m_InputMod->QueryInput(Input::Q))
		change.y -= 1;
	if (m_InputMod->QueryInput(Input::E))
		change.y += 1;
	if (m_InputMod->QueryInput(Input::LEFT))
		rotation.x -= 1;
	if (m_InputMod->QueryInput(Input::RIGHT))
		rotation.x += 1;
	if (m_InputMod->QueryInput(Input::UP))
		rotation.y -= 1;
	if (m_InputMod->QueryInput(Input::DOWN))
		rotation.y += 1;
	
	rotation *= 3.14f / 180;
	m_Camera->Rotate(rotation.x, rotation.y);
	if (change != glm::vec3(0))
		change = glm::normalize(change);
	m_Camera->Move(change * 0.05f);
	
}

void demo::SandboxDemo::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 mvp = m_Camera->GetWorldToScreenMatrix();
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
}
