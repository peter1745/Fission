#pragma once

#include "Buffer.hpp"

#include <filesystem>

namespace TestFramework {

	struct Vertex
	{
		float Position[3];
		float Normal[3];
	};

	struct Index
	{
		uint32_t Vertex0;
		uint32_t Vertex1;
		uint32_t Vertex2;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& InVertices, const std::vector<Index>& InIndices);
		~Mesh();

		Buffer* GetVertexBuffer() const { return m_VertexBuffer.get(); }
		Buffer* GetIndexBuffer() const { return m_IndexBuffer.get(); }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::unique_ptr<Buffer> m_VertexBuffer = nullptr;
		std::unique_ptr<Buffer> m_IndexBuffer = nullptr;
	};

}
