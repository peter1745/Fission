#include "FissionPCH.hpp"
#include "SphereShape.hpp"

namespace Fission {

	SphereShape::SphereShape(float InRadius)
	    : m_Radius(InRadius)
	{
		m_InertiaTensor = Math::Mat3x3(2.0f / 5.0f * InRadius * InRadius);

		m_BoundingBox.MinBound = Math::FVec3(-InRadius);
		m_BoundingBox.MaxBound = Math::FVec3(InRadius);
	}

}
