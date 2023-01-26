#include "FissionPCH.hpp"
#include "BoxShape.hpp"

namespace Fission {

	BoxShape::BoxShape(const Math::Vec3& InHalfSize)
	    : m_HalfSize(InHalfSize)
	{
		m_BoundingBox = { -InHalfSize, InHalfSize };

		/*float W = InHalfSize.GetX() * 2.0f;
		float H = InHalfSize.GetY() * 2.0f;
		float D = InHalfSize.GetZ() * 2.0f;

		m_InertiaTensor[0][0] = 1.0f / 12.0f * (W * W + D * D);
		m_InertiaTensor[1][1] = 1.0f / 12.0f * (D * D + H * H);
		m_InertiaTensor[2][2] = 1.0f / 12.0f * (W * W + H * H);*/
	}

}
