# Computer-Graphics-Redux

The third step in my journey to learn computer graphics from scratch, this is an evolution from my original [Computer Graphics project here](https://github.com/AlexWills37/ComputerGraphics)!

This is a demonstration of core computer graphics topics built with OpenGL and C++!

In the original project, I used SFML to draw pixels to the screen, and C++ to call the draw pixel function many times to implement a simple rasterizer, following Gabriel Gambetta's [Computer Graphics from Scratch textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/). I ended with unlit, multicolored, instanced and dynamic cubes, stopping at the *Shading* chapter.

## Demonstrations

### Rendering a sphere

<img width="462" alt="Wireframe sphere" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/3075953e-1a4e-4c3f-87c1-8a31fa551c2e">

> Generating the data for a sphere was the original roadblock that stopped
> me from continuing Gabriel Gambetta's [textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/).
> 
> With spheres, I could now explore different shading models with satisfying results!

<img width="466" alt="Solid sphere colored based on normals" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/a4ff89e6-27d7-42a1-8756-15cc8928f402">

> At first, I modeled diffuse lighting with a single directional light hard-coded in the fragment shader. This really gives the sphere depth!
>
> The sphere is colored based on its normals, which are remapped from the range [-1, 1] to the range [0, 1].
> 
> `vec4 normalColor = vec4((v_Normal * 0.5) + 0.5, 1.0f);`

### Gouraud shading

Gouraud shading calculates the lighting intensity at each vertex (here, in the vertex shader), then interpolating that value to the individual pixels.

<img width="467" alt="Highpoly gouraud shaded spheres" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/4fcecdc6-b3b5-4b51-9001-2e7795dc4e73">

> The lighting calculations now support multiple lights, and different types (point, directional, ambient)! In addition, now there are specular highlights!

<img width="460" alt="Lowpoly gouraud spheres" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/70bec22f-0446-466f-a9e1-eaaefe7c6d20">

> This model starts to look strange when the vertex count is low (fewer accurate lighting calculations), and when point lights get close to faces.

<img width="460" alt="Gouraud-lit cube" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/ef0e9be8-38b8-4dd0-9ee1-a4571576d210">

> In this render, the point light is directly in front of the yellow cube, but the face isn't lit very well! This is explained excellently in Gabriel Gambetta's [shading chapter](https://gabrielgambetta.com/computer-graphics-from-scratch/13-shading.html#gouraud-shading).

### Phong shading

Phong shading interpolates the vertices' positions and normals so that each pixel can run its own lighting computations. The result is much more accurate lighting, even with low poly shapes and close objects.

<img width="461" alt="Lowpoly phong shaded spheres" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/c52371b9-5deb-43ea-8523-4dec0d940441">

> Now, even low poly spheres have smooth lighting! Just don't look at the edges!

<img width="461" alt="Phong cube" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/4b204627-4c47-46ff-a8a5-33ae485a8b02">

> In this render, the point light is in about the same place as it was in the Gouraud shading, but now the entire face feels more properly lit.


### Textures

OpenGL does most of the heavy lifting with textures (generating mipmaps, bilinear filtering, interpolating (varying) variables with the z axis in mind), but that didn't make it any less satisfying to get textures working!

> The top and bottom faces use a dirt texture, while the side faces use the side of the grass texture. Both textures are in a single .png, like a texture atlas but only 2 textures!
> 
> > If the top face is a dirt texture, how is it green?
> >
> > This is definitely not optimal, but, working with what I had, I added some green to any pixels with a normal facing straight up. Just don't tilt the cubes!
> >
> > ``` 
> > if (v_Normal.y == 1) {
> >		texColor = sqrt(texColor) * vec4(0.6f, 1.0f, 0.6f, 1);
> >	}
> > ```



## OpenGL skills

### Uniform Buffer Objects

### Textures