#include "MeshGenerator.hpp"

#include <TestFramework/Renderer/Mesh.hpp>

#include <Fission/Math/Math.hpp>
#include <Fission/Math/FVec3.hpp>
#include <Fission/Math/FQuat.hpp>

MeshData MeshGenerator::GenerateCubeSphere(float radius, uint32_t segments, bool flipLRVertical, bool clockwise)
{
	std::vector<TestFramework::Vertex> vertices;
	std::vector<uint32_t> indices;

	// Front Back | Left Right | Top Bottom

	Fission::Math::FVec3 normals[] =
	{
		{ 0, 0, 1 }, { 0, 0, -1 }, { -1, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 }
	};
	
	Fission::Math::FVec3 tangentsNormal[] =
	{
		// Sides V -> +Y, Top and Bottom V -> -Z
		{ 1, 0, 0 }, { -1, 0, 0 }, { 0, 0, 1 }, { 0, 0, -1 }, { 1, 0, 0 }, { 1, 0, 0 }
	};
	
	Fission::Math::FVec3 tangentsFlipped[] =
	{
		// Front back V -> +Y, Left Right V -> -Y, Top Bottom V -> -Z
		{ 1, 0, 0 }, { -1, 0, 0 }, { 0, 0, -1 }, { 0, 0, 1 }, { 1, 0, 0 }, { 1, 0, 0 }
	};

	constexpr float PI = Fission::Math::PI;
	Fission::Math::FQuat rotations[] {
		Fission::Math::FQuat(Fission::Math::FVec3 { 0.f, 1.f, 0.f }, 0.f),
		Fission::Math::FQuat(Fission::Math::FVec3 { 0.f, 1.f, 0.f }, PI),

		Fission::Math::FQuat(Fission::Math::FVec3 { 0.f, 1.f, 0.f }, -0.5f * PI),
		Fission::Math::FQuat(Fission::Math::FVec3 { 0.f, 1.f, 0.f }, 0.5f * PI),

		Fission::Math::FQuat(Fission::Math::FVec3 { 1.f, 0.f, 0.f }, -0.5f * PI),
		Fission::Math::FQuat(Fission::Math::FVec3 { 1.f, 0.f, 0.f }, 0.5f * PI),
	};

	const Fission::Math::FVec3* tangents = flipLRVertical ? tangentsFlipped : tangentsNormal;

	for (uint32_t side = 0; side < 6; ++side)
	{
		auto N = normals[side];
		auto T = tangents[side];
		auto B = N.Cross(T);

		auto vertexOffset = uint32_t(vertices.size());

		float segmentSize = 2.f / segments;
		for (float dy = -1.f; dy <= 1.f; dy += segmentSize)
		{
			for (float dx = -1.f; dx <= 1.f; dx += segmentSize)
			{
				auto n = (N + (T * dx) + (B * dy)).Normalized();
				auto pos = n * radius;

				TestFramework::Vertex& vertex = vertices.emplace_back();
				memcpy(vertex.Position, &pos, sizeof(vertex.Position));
				memcpy(vertex.Normal, &n, sizeof(vertex.Normal));
				vertex.TextureCoordinate[0] = dx * 0.5f + 0.5f;
				vertex.TextureCoordinate[1] = dy * 0.5f + 0.5f;
			}
		}

		for (uint32_t latIndex = 0; latIndex < segments; ++latIndex)
		{
			uint32_t row1 = (segments + 1) * latIndex;
			uint32_t row2 = (segments + 1) * (latIndex + 1);

			for (uint32_t lonIndex = 0; lonIndex < segments; ++lonIndex)
			{
				uint32_t l1 = lonIndex;
				uint32_t l2 = lonIndex + 1;

				if (clockwise)
				{
					l1 = l2;
					l2 = lonIndex;
				}

				indices.push_back(vertexOffset + row1 + l2);
				indices.push_back(vertexOffset + row1 + l1);
				indices.push_back(vertexOffset + row2 + l1);

				indices.push_back(vertexOffset + row1 + l2);
				indices.push_back(vertexOffset + row2 + l1);
				indices.push_back(vertexOffset + row2 + l2);
			}
		}
	}

	TestFramework::BufferInfo VertexBufferInfo;
	VertexBufferInfo.InitialData = vertices.data();
	VertexBufferInfo.Size = vertices.size() * sizeof(TestFramework::Vertex);
	VertexBufferInfo.Usage = TestFramework::EBufferUsage::Vertex;

	MeshData result;
	result.VertexBuffer = std::make_unique<TestFramework::Buffer>(VertexBufferInfo);

	TestFramework::BufferInfo IndexBufferInfo;
	IndexBufferInfo.InitialData = indices.data();
	IndexBufferInfo.Size = indices.size() * sizeof(uint32_t);
	IndexBufferInfo.Usage = TestFramework::EBufferUsage::Index;
	result.IndexBuffer = std::make_unique<TestFramework::Buffer>(IndexBufferInfo);

	return result;
}
