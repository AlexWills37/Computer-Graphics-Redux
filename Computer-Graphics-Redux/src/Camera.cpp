#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(float screenWidth, float screenHeight, float fov, float nearPlaneDist, float farPlaneDist)
	: m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight), m_FOV(fov), m_NearPlaneDist(nearPlaneDist),
	m_FarPlaneDist(farPlaneDist)
{
	// Create projection matrix with perspective projection
	m_Projection = glm::perspective(
		glm::radians(fov), 
		screenWidth / screenHeight,       
		nearPlaneDist,             
		farPlaneDist
	);
}

Camera::~Camera()
{
	// All members may simply call their destructors
}

glm::mat4 Camera::GetWorldToScreenMatrix()
{
	glm::mat4 inverseCam = GetInverseCameraMatrix();
	return m_Projection * inverseCam;
}

void Camera::Rotate(float yawChange, float pitchChange)
{
	// Allow yaw to wrap around from 2pi to 0
	m_Yaw += yawChange;	
	m_Yaw = m_Yaw < 0 ? m_Yaw + 2 * 3.14f : (m_Yaw > 2 * 3.14f ? m_Yaw - 2 * 3.14f : m_Yaw);

	// Clamp pitch between 0 and pi
	m_Pitch += pitchChange;
	m_Pitch = m_Pitch > 3.14f ? 3.14f : (m_Pitch < 0 ? 0 : m_Pitch);
}

void Camera::Translate(glm::vec3 deltaPosition) {
	m_Position += deltaPosition;
}

void Camera::Move(glm::vec3 deltaPosition) {
	
	// Apply x translation on local x axis
	glm::vec3 localXDirec = glm::vec3(std::cosf(m_Yaw), 0, std::sinf(m_Yaw));
	m_Position += localXDirec * deltaPosition.x;

	// Apply y translation on global y axis
	m_Position.y += deltaPosition.y;

	// Apply z translation on the local z axis projected to the X/Z plane and normalized (forward on the X/Z plane)
	glm::vec3 flatZDirec = glm::vec3(-std::sinf(m_Yaw), 0, std::cosf(m_Yaw));
	m_Position += flatZDirec * deltaPosition.z;
}


glm::mat4 Camera::GetInverseCameraMatrix() {
	// Note: this code is far from optimized (intermediate steps and reassinging pitch/yaw to phi/theta not needed),
	//    but it is kept this way for understanding and future reference (this project is intended for demonstrating
	//    concepts, not actual use as a graphics/game engine).

	float phi = m_Pitch;
	float theta = m_Yaw;

	// Trust in the process :) this math works!
	// Solution inspired by https://gamedev.stackexchange.com/questions/168542/camera-view-matrix-from-position-yaw-pitch-worldup
	// Phi = pitch = rotation about local x axis. In range [0, pi], with 0 indicating the +y axis and pi indicating the -x axis
	// Theta = yaw = rotation about the global y axis. In range [0, 2pi] with 0 and 2pi representing no rotation
	/*
	 * Z axis is the inverse of the camera's "forward" direction (-Z in open gl).
	 *		Points will be in the projection matrix's frustum if z is negative (and in the FOV)
	 * X axis is always on the X/Z plane (since no roll is factored in, with the global "up" being the Y axis
	 * Y axis is perpendicular to the Z and X axes
	 */
	glm::vec3 zRotation = glm::vec3(-std::sinf(phi) * std::sinf(theta),
		-std::cosf(phi),
		std::sinf(phi) * std::cosf(theta));
	glm::vec3 xRotation = glm::vec3(std::cos(theta), 0, std::sin(theta));
	glm::vec3 yRotation = glm::cross(zRotation, xRotation);

	glm::mat4 rotationMatrix = glm::mat4(
		xRotation.x, xRotation.y, xRotation.z, 0,
		yRotation.x, yRotation.y, yRotation.z, 0,
		zRotation.x, zRotation.y, zRotation.z, 0,
		0, 0, 0, 1
	);

	glm::mat4 translationMatrix = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_Position.x, m_Position.y, m_Position.z, 1
	);

	glm::mat4 inverseTransform = glm::inverse(translationMatrix * rotationMatrix);

	return inverseTransform;
}
