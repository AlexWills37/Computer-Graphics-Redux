#version 330 core

// We are outputting color
// Layout location defaults to 0 anyway
layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
	//color = vec4(vec3(gl_FragCoord.z), 1.0);
}