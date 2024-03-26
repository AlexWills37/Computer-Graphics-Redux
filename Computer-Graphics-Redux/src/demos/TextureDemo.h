#pragma once
#include "GL/glew.h"
#include "demos/Demo.h"
#include "Input.h"
#include "Camera.h"
#include "Shader.h"

#include "Transform.h"


#include <memory>

namespace demo {
	class TextureDemo : public Demo {

	public:
		TextureDemo();
		~TextureDemo();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;

		struct Instance {
			Transform transform;
			float specValue;
			glm::vec3 color;
		};

		enum LightType : uint32_t {
			NONE = 0,
			AMBIENT = 1,
			DIRECTIONAL = 1 << 1,
			POINT = 1 << 2
		};

		struct Light
		{
			glm::vec3 position;
			LightType type;
			float intensity;
			glm::vec3 padding;
		};

	private:
		InputModule* m_InputMod;
		std::unique_ptr<Camera> m_Camera;
		uint32_t m_IndexCount;

		GLuint m_VAO, m_VertexBuffer, m_IndexBuffer, m_UniformLights, m_Texture;

		std::unique_ptr<Shader> m_Shader;

		std::vector<Instance> m_Scene;

		Light m_Lights[5];
	};
}
