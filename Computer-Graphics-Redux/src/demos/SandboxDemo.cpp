#include "SandboxDemo.h"

#include "Display.h"
#include "Model.h"


#include <iostream>

/*
 * Gouraud shading architecture:
 * Static object:
 *		Position
 *		Normal
 *		Color
 *		Base lighting (no specular)
 * 
 * Specular Lighting Requirements:
 *		All non-ambient light sources
 *			as Intensity and Light Vector (light - vertex)
 *		
 * 
 * Every frame:
 *		Add each vertex's specular lighting in vertex shader
 *		Set color in fragment shader
 *		
 * 
 * What the vertex shader receives:
 *		Base vertex
 *			Position, normal, color, base lighting
 *		Lighting information[] ???
 *			struct Light {
 *				float intensity;
 *				vec3 transform;
 *				float type;
 *			};
 *		Camera position
 *		Transform matrix
 *		
 *	
 */
namespace sandbox {
	struct LitVertex {
		Vertex v;
		float lightValue;
	};

	enum LightType {
		AMBIENT, POINT, DIRECTIONAL
	};

	struct Light {
		LightType type;
		float intensity;
		glm::vec3 transform;
	};

	float ComputeLighting(Vertex p, std::vector<Light> lights, glm::vec3 camPos)
	{
		float specValue = 1000;

		float totalLighting = 0;
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i].type == AMBIENT) {
				totalLighting += lights[i].intensity;
			}
			else {
				glm::vec3 lightVector;
				glm::vec3 normal = glm::vec3(p.normal[0], p.normal[1], p.normal[2]);
				glm::vec3 pointPos = glm::vec3(p.position[0], p.position[1], p.position[2]);
				if (lights[i].type == POINT) {
					lightVector = lights[i].transform - pointPos;
				}
				else if (lights[i].type == DIRECTIONAL) {
					lightVector = lights[i].transform;
				}
				else {
					std::cout << "This code should not run. Light Type unknown." << std::endl;
				}
				// Now we have the light vector
				float lightDotNorm = glm::dot(lightVector, normal);

				if (lightDotNorm > 0) {
					totalLighting += lights[i].intensity * lightDotNorm / (glm::length(lightVector) * glm::length(normal));
				}

				// Specular reflection
				if (specValue >= 0) {
					glm::vec3 reflectVector = 2.0f * normal * lightDotNorm - lightVector;
					glm::vec3 viewVector = pointPos - camPos;
					float reflectDotView = glm::dot(viewVector, reflectVector);
					if (reflectDotView > 0) {
						totalLighting += lights[i].intensity * glm::pow(reflectDotView / (glm::length(reflectVector) * glm::length(viewVector)), specValue);
					}
				}


			}
		}
		//std::cout << totalLighting << std::endl;
		return totalLighting;
	}
}
using namespace sandbox;
demo::SandboxDemo::SandboxDemo()
{
	// Get input module 
	Display* display = Display::GetDisplayPointer();
	m_InputMod = InputModule::GetInstance(display->GetWindow());

	// Create camera
	m_Camera = std::make_unique<Camera>(960, 500, 90, 0.1f, 100.0f);
	//m_Camera->Translate(glm::vec3(0, 3.0f, 0.0f));

	// Create Sphere data
	Model sphere = Model::CreateSphere(20);
	//Model sphere = Model::CreateCube();
	std::vector<uint32_t> indices = sphere.GetIndices();
	m_IndexCount = indices.size();
	std::vector<Vertex> vertices = sphere.GetVertices();

	VertexBufferLayout layout = sphere.GetLayout();
	
	// Add lighting information
	std::vector<Light> lights = {
		{AMBIENT, 0.2f},
		{DIRECTIONAL, 0.3f, glm::vec3(1, 3, 4)},
		{POINT, 0.6f, glm::vec3(2, 1, 2)}
	};

	glm::vec3 camPos = m_Camera->GetPosition();

	std::vector<LitVertex> litVertices;
	float lightValue = 1.0f;
	for (int i = 0; i < vertices.size(); i++) {
		lightValue = ComputeLighting(vertices[i], lights, camPos);
		litVertices.push_back({ vertices[i], lightValue });
	}
	layout.Push<float>(1);


	// Create vao
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create vb
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LitVertex), litVertices.data(), GL_STATIC_DRAW);
	

	const auto& attributes = layout.GetAttributes();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < attributes.size(); i++) {
		const auto& attribute = attributes[i];

		// Now use the layout to make attribute pointers to store in the current VAO
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attribute.count, attribute.type, attribute.normalized,
			sizeof(LitVertex), (const void*)offset);

		offset += attribute.count * VertexBufferAttribute::GetSizeOfType(attribute.type);
	}

	// Create index buffer
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	// Create Shader
	m_Shader = std::make_unique<Shader>("res/shaders/Gouraud");

	// Configue OpenGL things
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Move camera a little bit
	m_Camera->Translate(glm::vec3(0, 0, 3.0f));
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
