#version 330 core

// input attribute at index 0
// input attribute is a vec4 (will be cast) called position
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec3 v_Normal;
out vec4 v_Position;

uniform mat4 u_MVP;


void main()
{
	//gl_Position = vec4(position.xy, 1, 1);
	gl_Position = u_MVP * position;
	v_Normal = normal;
	v_Position = u_MVP * position;

}