#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
Transform::Transform()
	: m_Translation(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f))
{
}

Transform::Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
	: m_Translation(translation), m_Rotation(rotation), m_Scale(scale)
{
}

Transform::~Transform()
{
}

void Transform::Translate(glm::vec3 translation)
{
	m_Translation += translation;
}


glm::mat4 Transform::GetMatrix() const
{


	glm::mat4 scale = glm::scale(glm::mat4(1), m_Scale);
	glm::mat4 translation = glm::translate(glm::mat4(1), m_Translation);
	glm::mat4 rotation = glm::eulerAngleYXZ(m_Rotation[1], m_Rotation[0], m_Rotation[2]);

	//rotation = glm::mat4(1);

	glm::mat4 transformation = translation * rotation * scale;

	return transformation;
}
