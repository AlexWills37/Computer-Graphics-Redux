# Computer-Graphics-Redux

The third step in my journey to learn computer graphics from scratch, this is an evolution from my original [Computer Graphics project here](https://github.com/AlexWills37/ComputerGraphics)!

This is a demonstration of core computer graphics topics built with OpenGL and C++!

In the original project, I used SFML to draw pixels to the screen, and C++ to call the draw pixel function many times to implement a simple rasterizer, following Gabriel Gambetta's [Computer Graphics from Scratch textbook](https://gabrielgambetta.com/computer-graphics-from-scratch/). I ended with unlit, multicolored, instanced and dynamic cubes, stopping at the *Shading* chapter.

In this project, I started by rendering a cube with OpenGL. I then moved to generate and render a sphere with the same method, which then gave me the confidence to continue where the original project left off.

Below are some visual demonstrations of the concepts I explored.

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

Gouraud shading calculates the lighting intensity at each vertex (here, in the vertex shader), then interpolates that value to the individual pixels.

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
>
> The high poly version of this scene is left out, since it largely looks the same as it did with Gouraud shading.

<img width="461" alt="Phong cube" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/4b204627-4c47-46ff-a8a5-33ae485a8b02">

> In this render, the point light is in about the same place as it was in the Gouraud shading, but now the entire face feels more properly lit.


### Textures

OpenGL does most of the heavy lifting with textures (generating mipmaps, bilinear filtering, interpolating (varying) variables with the z axis in mind), but that didn't make it any less satisfying to get textures working!

<img width="472" alt="Grass blocks" src="https://github.com/AlexWills37/Computer-Graphics-Redux/assets/77563588/2051ff24-86b4-4e1e-a212-c0001dbaec3e">

> The top and bottom faces use a dirt texture, while the side faces use the side of the grass texture. Both textures are in a single .png, like a texture atlas but only 2 textures!
> 
> > If the top face is a dirt texture, how is it green?
> >
> > This is definitely not optimal, but, working with what I had, I added some green to any pixels with a normal facing straight up. Just don't tilt the cubes!
> >
> > ```glsl
> > if (v_Normal.y == 1) {
> >     texColor = sqrt(texColor) * vec4(0.6f, 1.0f, 0.6f, 1);
> > }
> > ```



## OpenGL skills

### Uniform Buffer Objects

This is my biggest takeaway from this project. Uniform Buffer Objects allow for using buffers to send large amounts of data as uniforms to shaders, instead of having to set every uniform individually.

> In [Phong.frag](./Computer-Graphics-Redux/res/shaders/Phong.frag), a fragment shader:

```glsl
struct Light 
{
    vec3 position;  // For directional lights, this is direction.
    int type;       // 0 = none, 1 = ambient, 1 < 1 = directional, 1 < 2 = point
    float intensity;
    // vec3 of padding
};

/* Uniforms */
layout(std140) uniform Lights 
{
    Light u_Lights[5];
};
```

In the shader, I have a Light struct with the information needed about a light. The comment `// vec3 of padding` is to help indicate how this struct is handled by the std140 layout. Structs in this layout are padded to be a multiple of 16 bytes = 4 floats = vec4. The vec3 and int make the first 16 bytes, and the `float intensity` only makes up 4 of the 16 bytes, leaving 12 bytes of added padding. In the array of lights, that padding shows up as empty space between each element.

Now to define this data on the CPU, we need a matching struct.

> From [PhongDemo.h](./Computer-Graphics-Redux/src/demos/PhongDemo.h):

```c++
enum LightType : uint32_t {
    NONE = 0,
    AMBIENT = 1,
    DIRECTIONAL = 1 << 1,
    POINT = 1 << 2
};

struct Light
{
    glm::vec3 position;
    LightType type;
    float intensity;
    glm::vec3 padding;
};
```

To make defining the scene easier, we can use an enum to translate between our LightTypes and the numbers that represent them. Then there is the struct, which has that extra vec3 of padding explicitly added. This has 2 benefits.

First, when creating the buffer for the lights, `sizeof(Light)` will be an accurate measurement.

Second, the array on the CPU will be spaced in the way our shader expects.

> From [PhongDemo.cpp](./Computer-Graphics-Redux/src/demos/PhongDemo.cpp):

```c++
// Add lighting information
m_Lights[0] = { glm::vec3(0, 0, 0), AMBIENT, 0.2f };
m_Lights[1] = { glm::vec3(-1, 3, 1), DIRECTIONAL, 0.3f };
m_Lights[2] = { glm::vec3(5, 3, -2), POINT, 0.6f };
m_Lights[3] = { glm::vec3(-1, -2, 3), POINT, 0.4f };

// ...

// Create uniform buffer for lights
glGenBuffers(1, &m_UniformLights);
glBindBuffer(GL_UNIFORM_BUFFER, m_UniformLights);
glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof(Light), NULL, GL_DYNAMIC_DRAW);
unsigned int blockIndex = glGetUniformBlockIndex(m_Shader->GetShaderID(), "Lights");
glUniformBlockBinding(m_Shader->GetShaderID(), blockIndex, 0);
glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UniformLights);
glBindBuffer(GL_UNIFORM_BUFFER, 0);

// ...

// Every frame to update lighting information:
glBindBuffer(GL_UNIFORM_BUFFER, m_UniformLights);
glBufferSubData(GL_UNIFORM_BUFFER, 0, 5 * sizeof(Light), &m_Lights[0]);
```

### Textures

With OpenGL, textures are relatively straightforward! OpenGL has access to a limited number of texture slots. After binding a texture to a texture slot, you can then connect it to a shader's uniform sampler by setting the uniform to the texture slot.

> From [TextureDemo.cpp](./Computer-Graphics-Redux/src/demos/TextureDemo.cpp):

```c++
// Create a texture and bind it to slot 0
glGenTextures(1, &m_Texture);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, m_Texture);

// Set texture parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

// Load texture data
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);

// Set shader uniform
m_Shader->SetUniform1i("u_Texture", 0);
```

Then in the shader, you can look up a texture's color value for a certain coordinate.

> From [LitTextures.frag](./Computer-Graphics-Redux/res/shaders/LitTextures.frag):

```glsl
uniform sampler2D u_Texture;

// ...

// Get the texture color
vec4 texColor = texture(u_Texture, v_TexCoords);
```

OpenGL takes care of the magic for deciding which mip to use if mipmaps are generated and enabled in the `GL_TEXTURE_MIN_FILTER` parameter. I suppose OpenGL knows how many pixels any given primitive are, and can compare that to the texture size to determine how to apply the chosen filtering rules.

## Closing Remarks

I look forward to continue working with OpenGL, possible working with Vulkan and implementing raytracing algorithms! Right now, I have 2 goals for the near future:

1. Use WebGL to make [a browser-based Tetris clone](https://github.com/AlexWills37/WebTetris).
2. Write shaders for environmental effects like water and scenic lighting.

As of writing this, I am making progress on the first goal and having a blast!
