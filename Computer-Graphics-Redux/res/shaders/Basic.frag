#version 330 core

// We are outputting color
// Layout location defaults to 0 anyway
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_Normal;



void main()
{

	float ambientLighting = 0.2f;
	float directionalLightingIntensity = 0.3f;
	vec3 directionalLightingDirection = vec3(1, 4, 4);

	float pixelLightIntensity = ambientLighting;
	float directionAddition = dot(v_Normal, directionalLightingDirection) / 
		(length(v_Normal) * length(directionalLightingDirection));
	directionAddition = clamp(directionAddition, 0.0f, 1.0f);
	pixelLightIntensity += directionAddition;


	//vec4 pcolor = vec4(0.2f, 0.5f, 0.9f, 1.0f);
	vec4 pcolor = vec4((v_Normal * 0.5) + 0.5, 1.0f);
	//color = vec4(angle, angle, angle, 1.0f);
	color = pcolor * pixelLightIntensity;
	//color = pcolor * 0.9f;
	//color = vec4(vec3(gl_FragCoord.z), 1.0);
	//color = v_Color;
}