#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

#include <iostream>

Camera::Camera(float screenWidth, float screenHeight, float fov, float nearPlaneDist, float farPlaneDist)
	: m_ScreenWidth(screenWidth), m_ScreenHeight(screenHeight), m_FOV(fov), m_NearPlaneDist(nearPlaneDist),
	m_FarPlaneDist(farPlaneDist), m_Transform(), m_Position(0)
{
	m_Projection = glm::perspective(
		glm::radians(fov), 
		screenWidth / screenHeight,       
		nearPlaneDist,             
		farPlaneDist
	);
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetWorldToScreenMatrix()
{
	glm::mat4 inverseCam = GetInverseCameraMatrix();
	return m_Projection * inverseCam;
}

void Camera::Rotate(float yawChange, float pitchChange)
{
	m_Yaw = m_Yaw + yawChange;	
	m_Yaw = m_Yaw < 0 ? m_Yaw + 2 * 3.14f : (m_Yaw > 2 * 3.14f ? m_Yaw - 2 * 3.14f : m_Yaw);
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

	float phi = m_Pitch;
	float theta = m_Yaw;

	// Trust in the process :) this math works
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

	//std::cout << "Z Vector: " << zRotation.x << ", " << zRotation.y << ", " << zRotation.z << std::endl;
	//std::cout << "X Vector: " << xRotation.x << ", " << xRotation.y << ", " << xRotation.z << std::endl;
	//std::cout << "Y Vector: " << yRotation.x << ", " << yRotation.y << ", " << yRotation.z << std::endl;

	glm::mat4 matrix = glm::mat4(glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(3));
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

	//glm::vec3 point = inverseTransform * glm::vec4(-1, -2, 5, 1);
	//std::cout << "point: " << point.x << ", " << point.y << ", " << point.z << std::endl;

	return inverseTransform;
}


glm::vec3 Camera::GetForwardVector(){
	glm::vec3 forward;
	forward.x = std::sin(m_Yaw);
	forward.y = 0;
	forward.z = -std::cos(m_Yaw);
	return forward;
}