#pragma once
#include "GL/glew.h"
#include "demos/Demo.h"
#include "Input.h"
#include "Camera.h"
#include "Shader.h"

#include <memory>

namespace demo {
	class NormalSphereDemo : public Demo {
	public:
		NormalSphereDemo();
		~NormalSphereDemo();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;

	private:
		InputModule* m_InputMod;
		std::unique_ptr<Camera> m_Camera;
		uint32_t m_IndexCount;

		GLuint m_VAO, m_VertexBuffer, m_IndexBuffer;

		std::unique_ptr<Shader> m_Shader;
	};
}
