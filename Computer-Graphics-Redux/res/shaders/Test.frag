#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec4 v_Position;


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


	vec4 pcolor = vec4(vec2(-v_Position.z * 0.5 + 0.5), 1.0f, 1.0f);
	color = pcolor;
}