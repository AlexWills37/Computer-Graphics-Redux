#pragma once
#include <vector>
#include <GL/glew.h>
#include <stdexcept>

/*
 * VertexBufferLayout.h
 * Contains the necessary information to define a layout for the vertices.
 */

/*
 * VertexBufferAttribute
 * Defines a single attribute of a vertex
 */
struct VertexBufferAttribute
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		}
		
		return 0;
	}
};

/*
 * VertexBufferLayout
 * Defines what a vertex is structured as, with a vector of Attributes.
 * 
 * Usage:
 *		Call Push<type>(count) to add an attribute to the layout.
 *		The attributes should be added in the order they appear in the vertex data
 */
class VertexBufferLayout
{
private:
	std::vector<VertexBufferAttribute> m_Attributes;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		throw std::runtime_error();
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_FLOAT);
	}
	
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Attributes.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferAttribute>& GetAttributes() const{ return m_Attributes; }
	inline unsigned int GetStride() const { return m_Stride; }
};