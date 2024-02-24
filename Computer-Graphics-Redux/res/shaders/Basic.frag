#version 330 core

// We are outputting color
// Layout location defaults to 0 anyway
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_Normal;



void main()
{
	vec3 thing = (v_Normal * 0.5) + 0.5;
	float angle = dot(v_Normal, vec3(0.1f, 0.9f, 0.3f));
	//color = vec4(angle, angle, angle, 1.0f);
	color = vec4(thing, 1.0f);
	//color = vec4(vec3(gl_FragCoord.z), 1.0);
	//color = vec4(v_Normal, 1.0f);
	//color = v_Color;
}