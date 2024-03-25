#version 330 core
/*
 * Phong Shading vertex shader.
 * This algorithm is performed per-pixel in the fragment shader.
 * This step just passes on the interpolated normal and world space coordinates
 * for the pixels.
 */

layout(location = 0) in vec4 position;	// Model space position
layout(location = 1) in vec3 normal;

out vec4 v_WorldPos;
out vec3 v_Normal;

/* Uniforms */
uniform mat4 u_TransformMatrix;
uniform mat4 u_ViewProj;

void main()
{
	v_WorldPos = u_TransformMatrix * position;
	v_Normal = normal;
	gl_Position = u_ViewProj * v_WorldPos;
}