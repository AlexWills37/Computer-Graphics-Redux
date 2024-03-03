#include "Model.h"
#include <cmath>

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
		{-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},	// back left bottom
		{-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
		{ 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
		{ 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
		
		
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
		{-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f},
		{-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f},
		{ 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f},

		// Right face
		{ 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f},
		{ 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
		{ 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
		{ 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f},

		// Left face
		{-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f},
		{-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
		{-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
		{-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f}
	};


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

Model Model::CreateSphere(uint32_t circlePointCount)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VertexBufferLayout layout;
	float increment = 3.14f / circlePointCount;
	int radius = 1;
	float theta, phi;
	
	

	
	for (int t = 0; t < circlePointCount * 2; t++) {
		theta = t * increment;
		for (int p = 0; p < circlePointCount; p++) {
			phi = p * increment;
			
			
			vertices.push_back(
				{
					radius * std::cos(theta) * std::sin(phi),	// x
					radius * std::sin(theta) * std::sin(phi),	// y
					radius * std::cos(phi),	// z

					// Normal
					std::cos(theta) * std::sin(phi),	// x
					std::sin(theta) * std::sin(phi),	// y
					std::cos(phi)	// z

				}
			);


			if (t != 0) {
				if (p != 0) {
					indices.push_back(p + (t - 1) * circlePointCount);	// Same point on previous ring
					indices.push_back(p + t * circlePointCount);	// Current point
					indices.push_back((p - 1) + t * circlePointCount);	// previous point on same ring

					indices.push_back(p + (t - 1) * circlePointCount);	// Same point on previous ring
					indices.push_back((p - 1) + t * circlePointCount);	// previous point on same ring
					indices.push_back((p - 1) + (t - 1) * circlePointCount);	// previous point on previous ring
				}
			}

			
			// Stitch the last ring with the first ring
			if (t == 2 * circlePointCount - 1) {	// At last ring, connect it back to the first ring
				
				if (p != 0) {
					indices.push_back((p - 1));						// Previous point on first ring
					indices.push_back((p - 1) + t * circlePointCount);	// Previous point on same ring
					indices.push_back(p + t * circlePointCount);	// Current point

					indices.push_back(p + t * circlePointCount);	// Current point
					indices.push_back(p );							// Same point on first ring
					indices.push_back((p - 1) );					// Preivoius point on first ring

				}
			}
			
		}
	}

	
	
	// Create the back cap
	theta = 0;
	phi = 3.14f;
	uint32_t backVertex = 2 * circlePointCount * circlePointCount;
	vertices.push_back(
		{
			radius * std::cos(theta) * std::sin(phi),	// x
			radius * std::sin(theta) * std::sin(phi),	// y
			radius * std::cos(phi),	// z

			// Normal
			std::cos(theta) * std::sin(phi),	// x
			std::sin(theta) * std::sin(phi),	// y
			std::cos(phi)	// z

		}
	);

	// Connect this last point with every final edge part
	for (int t = 0; t < circlePointCount * 2; t++) {
		int p = circlePointCount - 1 + t * circlePointCount;
		indices.push_back(p);
		indices.push_back(backVertex);
		indices.push_back((p + circlePointCount) % backVertex);
	}
	
	

	layout.Push<float>(3);
	layout.Push<float>(3);

	return Model(vertices, indices, layout);
}
