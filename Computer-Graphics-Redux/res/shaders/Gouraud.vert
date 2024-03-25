#version 330 core
/*
 * Gouraud shading calculates the light intensity at each vertex, then interpolates
 * that value for filling in the triangles.
 */

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float ambientLightIntensity;

out float v_Light;


struct Light 
{						// Memory (bytes)
	vec3 transform;		// 0-3
	float isDirectional;	// 4
	float intensity;	// 5
			// 6-7
};

// Get light information from cpu
layout (std140) uniform Lights
{
	Light lights[5];
};

// Get matrices from cpu
layout(std140) uniform Matrices
{
	mat4 u_ViewProj;
	vec4 u_CamPos;
};

// Per-object uniforms
uniform mat4 u_Transform;
uniform float u_SpecValue;

void main()
{
	// Get the point's position in the world, and the point on screen
	vec4 worldPosition = u_Transform * position;
	gl_Position = u_ViewProj * worldPosition;
	
	// Add light information
	float totalLightIntensity = ambientLightIntensity;
	for (int i = 0; i < 2; i++) {
		
		// Light direction vector
		vec3 lightVector;
		if (lights[i].isDirectional > 0.5f) {
			lightVector = lights[i].transform;
		} else {
			lightVector = lights[i].transform - worldPosition.xyz;
		}

		// Diffuse lighting
		float n_dot_l = dot(normal, lightVector);
		if (n_dot_l > 0) {
			totalLightIntensity += lights[i].intensity * n_dot_l / (length(normal) * length(lightVector));
		}
		
		// Specular lighting
		if (u_SpecValue >= 0) {
			vec3 reflectVector = 2.0f * normal * n_dot_l - lightVector;
			vec3 viewVector = u_CamPos.xyz - worldPosition.xyz;
			float r_dot_v = dot(reflectVector, viewVector);
			if (r_dot_v > 0) {
				totalLightIntensity += lights[i].intensity * pow(r_dot_v/(length(reflectVector) * length(viewVector)), u_SpecValue);
			}
		}
	}
	
	// Interpolate light intensity
	v_Light = totalLightIntensity;
}