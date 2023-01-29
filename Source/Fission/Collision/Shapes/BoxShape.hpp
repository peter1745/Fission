#pragma once

#include "Shape.hpp"
#include "Fission/Math/AABB.hpp"

namespace Fission {

	class BoxShape : public Shape
	{
	public:
		BoxShape(const Math::FVec3& InHalfSize);

		const Math::FVec3& GetHalfSize() const { return m_HalfSize; }
		const Math::AABB& GetBoundingBox() const { return m_BoundingBox; }

		constexpr EShapeType GetType() const override { return EShapeType::Box; }

	private:
		Math::FVec3 m_HalfSize;
		Math::AABB m_BoundingBox;
	};

}
