#version 330 core
/*
 * Phong Shading fragment shader.
 * With the world position and normal interpolated from the vertices,
 * as well as a list of the scene's lights, each pixel can calculate
 * its own lighting intensity for accurate lighting.
 */

layout(location = 0) out vec4 color;

in vec4 v_WorldPos;
in vec3 v_Normal;

struct Light 
{
	vec3 position;	// For directional lights, this is direction.
	int type;	// 0 = none, 1 = ambient, 1 < 1 = directional, 1 < 2 = point
	float intensity;
	// vec3 of padding
};

/* Uniforms */
layout(std140) uniform Lights 
{
	Light u_Lights[5];
};
uniform vec4 u_Color;
uniform float u_SpecValue;
uniform vec4 u_CamPos;

float ComputeLighting(Light light) {
	float intensity = 0;
	
	if (light.type == 0)
		return 0;
	if ((light.type & 1) != 0) { // Ambient light
		intensity += light.intensity;
	} else {
		// Get the vector from the point to the light
		vec3 lightDirection;
		if ((light.type & (1 << 2)) != 0) { // Point light
			lightDirection = light.position - v_WorldPos.xyz;
		} else {	// Directional light
			lightDirection = light.position;
		}

		// Diffuse lighting
		float n_dot_l = dot(v_Normal, lightDirection);
		if (n_dot_l > 0) {
			intensity += light.intensity * n_dot_l / (length(v_Normal) * length(lightDirection));
		}

		// Specular lighting
		if (u_SpecValue > -1) {
			vec3 reflection = 2.0f * v_Normal * n_dot_l - lightDirection;
			vec3 viewVector = u_CamPos.xyz - v_WorldPos.xyz;
			float r_dot_v = dot(reflection, viewVector);
			if (r_dot_v > 0) {
				intensity += light.intensity * pow(r_dot_v / (length(reflection) * length(viewVector)), u_SpecValue);
			}
		}
	}

	return intensity;
}

void main()
{

	// Calculate lighting
	float totalLighting = 0;
	for (int i = 0; i < 5; i++) {
		totalLighting += ComputeLighting(u_Lights[i]);
	}

	color = u_Color * totalLighting;
}