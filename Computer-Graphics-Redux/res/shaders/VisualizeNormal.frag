#version 330 core

// We are outputting color
// Layout location defaults to 0 anyway
layout(location = 0) out vec4 color;

in vec3 v_Normal;

void main()
{
	float ambientLighting = 0.2f;
	float directionalLightingIntensity = 0.3f;
	vec3 directionalLightingDirection = vec3(1, 4, 4);


	float pixelLightIntensity = ambientLighting;
	float directionAddition = dot(v_Normal, directionalLightingDirection) / 
		(length(v_Normal) * length(directionalLightingDirection));
	directionAddition = clamp(directionAddition, 0.0f, 1.0f);	// Do not have negative light
	pixelLightIntensity += directionAddition;


	// Visualize normal by remapping values from [-1, 1] to [0, 1]
	vec4 normalColor = vec4((v_Normal * 0.5) + 0.5, 1.0f);
	
	color = normalColor * pixelLightIntensity;
}