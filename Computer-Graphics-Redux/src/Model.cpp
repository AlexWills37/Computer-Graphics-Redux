#include "Model.h"


Model::Model(std::vector<Vertex> vertices, std::vector<uint32_t> indices, VertexBufferLayout layout)
	: m_Vertices(vertices), m_Indices(indices), m_Layout(layout), m_IndexCount(indices.size())
{
}

Model::~Model()
{
}

Model Model::CreateCube()
{
	std::vector<Vertex> vertices = {

		// Back face
		{-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},	// back left bottom
		{-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
		
		
		// Top face
		{-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
		{-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f},
		{ 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
		
		// Front face
		{-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f},
		{-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f},
		{ 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f},
		{ 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f},

		// Bottom face
		{-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f},
		{-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f},
		{ 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f},

		// Right face
		{ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f},
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f},
		{ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f},
		{ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f},

		// Left face
		{-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f},
		{-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
		{-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
		{-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f}
	};

	float width = 960;
	float height = 540;
	float viewportDistance = 0.5;
	float viewport_height = 1;
	float aspect_ratio = float(width) / float(height);
	float viewport_width = viewport_height * aspect_ratio;

	/*
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].position[2] = vertices[i].position[2] - 1.5;
		vertices[i].position[0] = vertices[i].position[0] + 0.2;
		vertices[i].position[1] = vertices[i].position[1] + 0.2;
		
	}*/

	std::vector<uint32_t> indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);


	return Model(vertices, indices, layout);
}
