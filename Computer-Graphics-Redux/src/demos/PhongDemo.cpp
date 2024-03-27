#include "PhongDemo.h"

#include "Display.h"
#include "Model.h"

#include <iostream>

/*
 * Phong shading uses the same light computations, but now calculates the light intensity
 * at every pixel (compared to Gouraud shading, which does it at every vertex).
 * To achieve this, we interpolate each vertex's
 *   - Normal
 *   - World position
 * I believe OpenGL does 3D interpolation with varying attributes, so we do not need
 * to use any tricks to retrieve each pixel's world space.
 */
demo::PhongDemo::PhongDemo()
{
	// Get input module 
	Display* display = Display::GetDisplayPointer();
	m_InputMod = InputModule::GetInstance(display->GetWindow());

	// Create camera
	m_Camera = std::make_unique<Camera>(960, 500, 45, 0.1f, 100.0f);
	m_Camera->Translate(glm::vec3(-1, 2.0f, 8.0f));

	// Create Sphere data
	//Model sphere = Model::CreateSphere(10);
	Model sphere = Model::CreateCube();
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


	/* Build Scene */
	Transform a = Transform(glm::vec3(-5, 3, -1), glm::vec3(0), glm::vec3(1));
	Transform b = Transform(glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(1));
	Transform c = Transform(glm::vec3(5, 3, -4), glm::vec3(0), glm::vec3(3));
	m_Scene.push_back({ a, 10, glm::vec3(0.2, 0.8, 0.9) });
	m_Scene.push_back({ b, 500, glm::vec3(0.9, 0.3, 0.1) });
	m_Scene.push_back({ c, 1000, glm::vec3(0.9, 0.8, 0.2) });


	// Add lighting information
	m_Lights[0] = { glm::vec3(0, 0, 0), AMBIENT, 0.2f };
	m_Lights[1] = { glm::vec3(-1, 3, 1), DIRECTIONAL, 0.3f };
	m_Lights[2] = { glm::vec3(5, 3, -2), POINT, 0.6f };
	//m_Lights[3] = { glm::vec3(-1, -2, 3), POINT, 0.4f };

	// Create Shader
	m_Shader = std::make_unique<Shader>("res/shaders/Phong");

	// Create uniform buffer for lights
	glGenBuffers(1, &m_UniformLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformLights);
	glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof(Light), NULL, GL_DYNAMIC_DRAW);
	unsigned int blockIndex = glGetUniformBlockIndex(m_Shader->GetShaderID(), "Lights");
	glUniformBlockBinding(m_Shader->GetShaderID(), blockIndex, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UniformLights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Configue OpenGL things
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

demo::PhongDemo::~PhongDemo()
{
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_UniformLights);
	glDeleteVertexArrays(1, &m_VAO);
}

void demo::PhongDemo::OnUpdate(float deltaTime)
{
	// Move camera
	glm::vec3 deltaPos = glm::vec3(0);
	glm::vec2 deltaRot = glm::vec2(0);
	float speedModifier = 1;
	
	if (m_InputMod->QueryInput(Input::S))
		deltaPos.z += 1;
	if (m_InputMod->QueryInput(Input::W))
		deltaPos.z -= 1;
	if (m_InputMod->QueryInput(Input::A))
		deltaPos.x -= 1;
	if (m_InputMod->QueryInput(Input::D))
		deltaPos.x += 1;
	if (m_InputMod->QueryInput(Input::Q))
		deltaPos.y -= 1;
	if (m_InputMod->QueryInput(Input::E))
		deltaPos.y += 1;
	if (m_InputMod->QueryInput(Input::LEFT))
		deltaRot.x -= 1;
	if (m_InputMod->QueryInput(Input::RIGHT))
		deltaRot.x += 1;
	if (m_InputMod->QueryInput(Input::UP))
		deltaRot.y -= 1;
	if (m_InputMod->QueryInput(Input::DOWN))
		deltaRot.y += 1;
	if (m_InputMod->QueryInput(Input::SHIFT))
		speedModifier = 2;
	if (m_InputMod->QueryInput(Input::CONTROL))
		speedModifier = 0.5f;

	deltaRot *= 3.14f / 180;
	m_Camera->Rotate(deltaRot.x, deltaRot.y);
	if (deltaPos != glm::vec3(0))
		deltaPos = glm::normalize(deltaPos) * speedModifier;
	m_Camera->Move(deltaPos * 0.05f);

	// Move point light
	glm::vec3 deltaLightPos = glm::vec3(0, 0, 0);
	if (m_InputMod->QueryInput(Input::I))
		deltaLightPos.z -= 1;
	if (m_InputMod->QueryInput(Input::J))
		deltaLightPos.x -= 1;
	if (m_InputMod->QueryInput(Input::K))
		deltaLightPos.z += 1;
	if (m_InputMod->QueryInput(Input::L))
		deltaLightPos.x += 1;
	deltaLightPos *= 0.1f;
	m_Lights[2].position += deltaLightPos;
	
}

void demo::PhongDemo::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Uniforms for the scene: camera position, viewproj matrix, lights
	glm::vec3 camPos = m_Camera->GetPosition();
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_CamPos", camPos.x, camPos.y, camPos.z, 1.0f);
	m_Shader->SetUniformMat4f("u_ViewProj", m_Camera->GetWorldToScreenMatrix());
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 5 * sizeof(Light), &m_Lights[0]);

	for (int i = 0; i < m_Scene.size(); i++) {
		// Uniforms for the object: transform, color, specular value
		m_Shader->SetUniformMat4f("u_TransformMatrix", m_Scene[i].transform.GetMatrix());
		m_Shader->SetUniform4f("u_Color", m_Scene[i].color.r, m_Scene[i].color.g, m_Scene[i].color.b, 1.0f);
		m_Shader->SetUniform1f("u_SpecValue", m_Scene[i].specValue);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
	}


}
