#version 330 core

// input attribute at index 0
// input attribute is a vec4 (will be cast) called position
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 v_Color;

uniform mat4 u_MVP;


void main()
{
	//gl_Position = vec4(position.xy, 1, 1);
	gl_Position = u_MVP * position;
	v_Color = color;

}