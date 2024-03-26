#version 330 core
/*
 * Phong Shading vertex shader.
 * This algorithm is performed per-pixel in the fragment shader.
 * This step just passes on the interpolated normal and world space coordinates
 * for the pixels.
 */

layout(location = 0) in vec4 position;	// Model space position
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec4 v_WorldPos;
out vec3 v_Normal;
out vec2 v_TexCoords;

/* Uniforms */
uniform mat4 u_TransformMatrix;
uniform mat4 u_ViewProj;

void main()
{
	v_WorldPos = u_TransformMatrix * position;
	v_Normal = normal;
	v_TexCoords = texCoords;
	gl_Position = u_ViewProj * v_WorldPos;
}