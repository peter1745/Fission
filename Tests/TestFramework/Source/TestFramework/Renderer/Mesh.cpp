#include "Mesh.hpp"

namespace TestFramework {

	Mesh::Mesh(const std::vector<Vertex>& InVertices, const std::vector<Index>& InIndices)
		: m_Vertices(InVertices), m_Indices(InIndices)
	{
		BufferInfo vertexBufferInfo = {};
		vertexBufferInfo.Usage = EBufferUsage::Vertex;
		vertexBufferInfo.Size = InVertices.size() * sizeof(Vertex);
		vertexBufferInfo.InitialData = m_Vertices.data();
		m_VertexBuffer = std::make_unique<Buffer>(vertexBufferInfo);

		BufferInfo indexBufferInfo = {};
		indexBufferInfo.Usage = EBufferUsage::Index;
		indexBufferInfo.Size = InIndices.size() * sizeof(Index);
		indexBufferInfo.InitialData = m_Indices.data();
		m_IndexBuffer = std::make_unique<Buffer>(indexBufferInfo);
	}

	Mesh::~Mesh()
	{

	}

}