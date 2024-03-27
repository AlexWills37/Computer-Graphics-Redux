# Computer-Graphics-Redux

The third step in my journey to learn computer graphics from scratch, this is an evolution from my original [Computer Graphics project here](https://github.com/AlexWills37/ComputerGraphics)!

This is a demonstration of core computer graphics topics built with OpenGL and C++!

In the original project, I used SFML to draw pixels to the screen, and C++ to call the draw pixel function many times to implement a simple rasterizer, following Gabriel Gambetta's [Computer Graphics from Scratch textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/). I ended with unlit, multicolored, instanced and dynamic cubes, stopping at the *Shading* chapter.

## Demonstrations

### Rendering a sphere

> Generating the data for a sphere was the original roadblock that stopped
> me from continuing Gabriel Gambetta's [textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/).
> 
> With spheres, I could now explore different shading models with satisfying results!

> At first, I modeled diffuse lighting with a single directional light hard-coded in the fragment shader. This really gives the sphere depth!

### Gouraud shading

Gouraud shading calculates the lighting intensity at each vertex (here, in the vertex shader), then interpolating that value to the individual pixels.

> The lighting calculations now support multiple lights, and different types (point, directional, ambient)! In addition, now there are specular highlights!

> This model starts to look strange when the vertex count is low (fewer accurate lighting calculations), and when point lights get close to faces.

> In this render, the point light is directly in front of the yellow cube, but the face isn't lit very well! This is explained excellently in Gabriel Gambetta's [shading chapter](https://gabrielgambetta.com/computer-graphics-from-scratch/13-shading.html#gouraud-shading).

### Phong shading

Phong shading interpolates the vertices' positions and normals so that each pixel can run its own lighting computations. The result is much more accurate lighting, even with low poly shapes and close objects.

> Now, even low poly spheres have smooth lighting! Just don't look at the edges!

> In this render, the point light is in about the same place as it was in the Gouraud shading, but now the entire face feels more properly lit.

