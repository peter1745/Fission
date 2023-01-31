#pragma once

#include "TestFramework/Core/Core.hpp"
#include "CommonTypes.hpp"
#include "Buffer.hpp"

namespace TestFramework {
	
	struct VertexBufferView
	{
		Buffer* VertexBuffer;
		uint32_t VertexStride;
	};

	struct IndexBufferView
	{
		Buffer* IndexBuffer;
		EFormat IndexFormat;
	};
	
}
