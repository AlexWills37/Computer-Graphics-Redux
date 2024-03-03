#version 330 core
/*
 * Gouraud shading calculates the light intensity at each vertex, then interpolates
 * that value for filling in the triangles.
 *
 * With a lighting value interpolated from the vertex shader, we multiply the object's base color
 * with the lighting intensity.
 */


layout(location = 0) out vec4 color;

in float v_Light;

uniform vec4 u_Color;

void main()
{
	color = u_Color * v_Light;
}