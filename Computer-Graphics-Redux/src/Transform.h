#pragma once
#include "glm/glm.hpp"

class Transform {
private:
	glm::vec3 m_Translation, m_Rotation, m_Scale;
public:
	Transform();
	Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
	~Transform();

	void Translate(glm::vec3 translation);
	void Rotate(float amount);

	glm::mat4 GetMatrix() const;
};