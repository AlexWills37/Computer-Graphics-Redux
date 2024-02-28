#pragma once
#include "glm/glm.hpp"

/*
 * Represents a camera object, which is manages the transformation matrices
 * for converting points from world space to Normalized Device Coordinates.
 * 
 * The camera contains the World-to-Camera space matrix as well as the
 * Projection (Camera-to-Screen) matrix because they are functionally linked
 * (in a single frame, these matrices only need to be computed once).
 * 
 * Currently uses a perspective projection matrix. Future work could extend this
 * class to additionally use an orthographic projection if desired.
 * 
 * @author Alex Wills
 * @date February 27, 2024
 */
class Camera {
private:
	float m_ScreenWidth, m_ScreenHeight, m_FOV, m_NearPlaneDist, m_FarPlaneDist;
	glm::mat4 m_Projection;
	float m_Yaw = 0, m_Pitch = 3.14f/2;
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

	/*
	 * Rotates the camera with respect to pitch and yaw (radians).
	 * 
	 * @param yawChange - rotation about the global Y axis (left/right)
	 * @param pitchChange - rotation about the local X axis (up/down)
	 */
	void Rotate(float yawChange, float pitchChange);

	/*
	 * Changes the camera's position, using world space coordinate axes.
	 * (x, y, and z directly correspond to the camera's coordinates).
	 */
	void Translate(glm::vec3 deltaPosition);

	/*
	 * Changes the camera's position according to it's local forward direction.
	 * (x moves left/right in world space from the camera's POV,
	 *  y moves along the global y axis, &
	 *  z moves forward/back from the camera's POV, but on the global X/Y plane, like in an FPS game).
	 */
	void Move(glm::vec3 deltaPosition);
	glm::mat4 GetInverseCameraMatrix();
};
