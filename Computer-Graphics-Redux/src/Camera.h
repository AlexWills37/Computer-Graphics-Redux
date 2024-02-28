#pragma once
#include "Transform.h"
#include "glm/glm.hpp"

class Camera {
private:
	Transform m_Transform;
	float m_ScreenWidth, m_ScreenHeight, m_FOV, m_NearPlaneDist, m_FarPlaneDist;
	glm::mat4 m_Projection;
	float m_Yaw = 0, m_Pitch = 0;
	glm::vec3 m_Position;

public:
	// FOV is in degrees
	Camera(float screenWidth, float screenHeight, float fov, float nearPlaneDist, float farPlaneDist);
	~Camera();

	/*
	 * Returns the combined View and Projection matricies to
	 * translate points from world space to normalized device space in range (-1, 1)
	 */
	glm::mat4 GetWorldToScreenMatrix();

	void Rotate(float yawChange, float pitchChange);
	void Translate(glm::vec3 deltaPosition);
	void Move(glm::vec3 deltaPosition);

	glm::vec3 GetForwardVector();

	glm::mat4 GetInverseCameraMatrix();
private:
};
