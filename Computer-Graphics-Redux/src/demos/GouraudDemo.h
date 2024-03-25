#pragma once
#include "GL/glew.h"
#include "demos/Demo.h"
#include "Input.h"
#include "Camera.h"
#include "Shader.h"
#include "Transform.h"

#include <memory>

namespace demo {


	class GouraudDemo : public Demo {
	public:
		struct SphereInstance {
			Transform transform;
			float specValue;
			glm::vec3 color;
		};
		enum LightType {
			AMBIENT, POINT, DIRECTIONAL
		};

		struct Light {
			LightType type;
			float intensity;
			glm::vec3 transform;
		};
	public:
		GouraudDemo();
		~GouraudDemo();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;

	private:
		InputModule* m_InputMod;
		std::unique_ptr<Camera> m_Camera;
		uint32_t m_IndexCount;
		std::vector<SphereInstance> m_Scene;

		GLuint m_VAO, m_VertexBuffer, m_IndexBuffer, m_UniformMatrices, m_UniformLights;

		std::unique_ptr<Shader> m_Shader;

		std::vector<Light> m_Lights;
		glm::vec3 m_PointLightPos;
	};
}