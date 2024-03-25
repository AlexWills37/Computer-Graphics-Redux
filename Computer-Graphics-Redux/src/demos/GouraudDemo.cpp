#include "GouraudDemo.h"

#include "GouraudDemo.h"

#include "Display.h"
#include "Model.h"
#include "Transform.h"


#include <iostream>
#include <glm/gtc/type_ptr.hpp>

struct UniformLight {
	glm::vec3 transform;		// 0-3
	float isDirectional;	// 4
	float intensity;	// 5
	glm::vec3 padding06;
};

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
 *			Position, normal, base lighting
 *		Lighting information[] ???
 *			struct Light {
 *				float intensity;
 *				vec3 transform;
 *				float type;
 *			};
 *		Camera position
 *		Transform matrix
 *
 * Scene is list of models AKA -
 *		VAO with vertex info
 *		Draw call
 *		Shader uniforms
 *			here specular value (s)
 *			mvp matrix
 *			camera position
 *			lights[]
 *			color
 *			
 */



struct LitVertex {
	Vertex v;
	float lightValue;
};

using namespace demo;

float ComputeLighting(Vertex p, std::vector<GouraudDemo::Light> lights, glm::vec3 camPos)
{
	float specValue = 1000;

	float totalLighting = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i].type == GouraudDemo::AMBIENT) {
			totalLighting += lights[i].intensity;
		}
		else {
			glm::vec3 lightVector;
			glm::vec3 normal = glm::vec3(p.normal[0], p.normal[1], p.normal[2]);
			glm::vec3 pointPos = glm::vec3(p.position[0], p.position[1], p.position[2]);
			if (lights[i].type == GouraudDemo::POINT) {
				lightVector = lights[i].transform - pointPos;
			}
			else if (lights[i].type == GouraudDemo::DIRECTIONAL) {
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

float ComputePartialLighting(Vertex p, std::vector<GouraudDemo::Light> lights)
{

	float totalLighting = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i].type == GouraudDemo::AMBIENT) {
			totalLighting += lights[i].intensity;
		}
		else {
			glm::vec3 lightVector;
			glm::vec3 normal = glm::vec3(p.normal[0], p.normal[1], p.normal[2]);
			glm::vec3 pointPos = glm::vec3(p.position[0], p.position[1], p.position[2]);
			if (lights[i].type == GouraudDemo::POINT) {
				lightVector = lights[i].transform - pointPos;
			}
			else if (lights[i].type == GouraudDemo::DIRECTIONAL) {
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
		}
	}
	return totalLighting;
}

float ComputeAmbientLighting(std::vector<GouraudDemo::Light> lights) {
	float l = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i].type == GouraudDemo::AMBIENT)
			l += lights[i].intensity;
	}
	return l;
}

demo::GouraudDemo::GouraudDemo()
{
	// Get input module 
	Display* display = Display::GetDisplayPointer();
	m_InputMod = InputModule::GetInstance(display->GetWindow());

	// Create camera
	m_Camera = std::make_unique<Camera>(960, 500, 45, 0.1f, 100.0f);
	//m_Camera->Translate(glm::vec3(0, 3.0f, 0.0f));



	/* Build Scene */
	Transform a = Transform(glm::vec3(-5, 3, -1), glm::vec3(0), glm::vec3(1));
	Transform b = Transform(glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(1));
	Transform c = Transform(glm::vec3(5, 3, -4), glm::vec3(0), glm::vec3(3));
	m_Scene.push_back({ a, 10, glm::vec3(0.2, 0.8, 0.9) });
	m_Scene.push_back({ b, 500, glm::vec3(0.9, 0.3, 0.1) });
	m_Scene.push_back({ c, 1000, glm::vec3(0.9, 0.8, 0.2) });

	// Create Sphere data
	Model sphere = Model::CreateSphere(200);
	//Model sphere = Model::CreateCube();
	std::vector<uint32_t> indices = sphere.GetIndices();
	m_IndexCount = indices.size();
	std::vector<Vertex> vertices = sphere.GetVertices();

	VertexBufferLayout layout = sphere.GetLayout();

	// Add partial lighting information to vertices
	m_Lights = {
		{AMBIENT, 0.2f},
		{DIRECTIONAL, 0.3f, glm::vec3(-1, 3, 1)},
		{POINT, 0.6f, glm::vec3(0, 1, -2)}
	};

	//glm::vec3 camPos = m_Camera->GetPosition();

	std::vector<LitVertex> litVertices;
	float lightValue = ComputeAmbientLighting(m_Lights);
	for (int i = 0; i < vertices.size(); i++) {
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


	// Create uniform buffer for matrices
	glGenBuffers(1, &m_UniformMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	unsigned int matrixIndex = glGetUniformBlockIndex(m_Shader->GetShaderID(), "Matrices");
	glUniformBlockBinding(m_Shader->GetShaderID(), matrixIndex, 0);	// Bind the "Matrices" (in the shader) to point 0 (in the Uniform Buffer).

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UniformMatrices); // Connect index 0 to the buffer we made

	// Create uniform buffer for lights
	glGenBuffers(1, &m_UniformLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformLights);
	glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof(UniformLight), NULL, GL_STATIC_DRAW);

	unsigned int lightsIndex = glGetUniformBlockIndex(m_Shader->GetShaderID(), "Lights");
	glUniformBlockBinding(m_Shader->GetShaderID(), lightsIndex, 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_UniformLights);

	// Add light info
	UniformLight lights[2] = {
		{m_Lights[1].transform, m_Lights[1].type == DIRECTIONAL ? 1.0f : 0.0f, m_Lights[1].intensity},
		{m_Lights[2].transform, m_Lights[2].type == DIRECTIONAL ? 1.0f : 0.0f, m_Lights[2].intensity}
	};
	glBufferSubData(GL_UNIFORM_BUFFER, 0,  2 * sizeof(UniformLight), &lights[0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_PointLightPos = lights[1].transform;
}

demo::GouraudDemo::~GouraudDemo()
{
	glDeleteBuffers(1, &m_IndexBuffer);
	glDeleteBuffers(1, &m_VertexBuffer);
	glDeleteBuffers(1, &m_UniformMatrices);
	glDeleteBuffers(1, &m_UniformLights);
	glDeleteVertexArrays(1, &m_VAO);
}

void demo::GouraudDemo::OnUpdate(float deltaTime)
{
	glm::vec3 change = glm::vec3(0);
	glm::vec2 rotation = glm::vec2(0);
	// Update camera position
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

	// Update light position
	if (m_InputMod->QueryInput(Input::I))
		m_PointLightPos.z -= 0.1f;
	if (m_InputMod->QueryInput(Input::K))
		m_PointLightPos.z += 0.1f;
	if (m_InputMod->QueryInput(Input::J))
		m_PointLightPos.x -= 0.1f;
	if (m_InputMod->QueryInput(Input::L))
		m_PointLightPos.x += 0.1f;

	rotation *= 3.14f / 180;
	m_Camera->Rotate(rotation.x, rotation.y);
	if (change != glm::vec3(0))
		change = glm::normalize(change);
	if (m_InputMod->QueryInput(Input::SHIFT))
		change *= 2.0f;
	if (m_InputMod->QueryInput(Input::CONTROL))
		change *= 0.5f;
	m_Camera->Move(change * 0.05f);

}

void demo::GouraudDemo::OnRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Get camera position and view-projection matrix
	glm::mat4 viewProj = m_Camera->GetWorldToScreenMatrix();
	glm::vec3 camPos = m_Camera->GetPosition();

	// Camera pos and light info will stay constant
	m_Shader->Bind();

	// Update uniform buffer objects
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProj));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(camPos));

	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformLights);
	glBufferSubData(GL_UNIFORM_BUFFER, (sizeof(UniformLight)), sizeof(glm::vec3), glm::value_ptr(m_PointLightPos));

	// Render each object
	for (int i = 0; i < m_Scene.size(); i++) {
		m_Shader->SetUniformMat4f("u_Transform", m_Scene[i].transform.GetMatrix());
		glm::vec3 color = m_Scene[i].color;
		m_Shader->SetUniform4f("u_Color", color.r, color.g, color.b, 1.0f);
		m_Shader->SetUniform1f("u_SpecValue", m_Scene[i].specValue);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
	}
}
