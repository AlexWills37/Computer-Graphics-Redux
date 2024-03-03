#pragma once
#include <vector>
#include "VertexBufferLayout.h"

struct Vertex {
	float position[3];
	float normal[3];
};

class Model {
private:
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	VertexBufferLayout m_Layout;
	size_t m_IndexCount;

public:
	Model(std::vector<Vertex> vertices, std::vector<uint32_t> indices, VertexBufferLayout layout);
	~Model();

	inline VertexBufferLayout GetLayout() { return m_Layout; }
	inline std::vector<Vertex> GetVertices() {return m_Vertices; }
	inline std::vector<uint32_t> GetIndices() { return m_Indices; }
	inline size_t GetIndexCount() { return m_IndexCount; }


	static Model CreateCube();
	static Model CreateSphere(uint32_t circlePointCount);
};
