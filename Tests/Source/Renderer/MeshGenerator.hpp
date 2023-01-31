#pragma once

#include <TestFramework/Renderer/Buffer.hpp>

struct MeshData
{
	std::unique_ptr<TestFramework::Buffer> VertexBuffer = nullptr;
	std::unique_ptr<TestFramework::Buffer> IndexBuffer = nullptr;
};

class MeshGenerator
{
public:
	static MeshData GenerateCubeSphere(float radius, uint32_t segments, bool flipLRVertical, bool clockwise);
};
