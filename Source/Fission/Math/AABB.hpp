#pragma once

#include "Vec3.hpp"

namespace Fission::Math {

	struct AABB
	{
		Vec3 MinBound;
		Vec3 MaxBound;

		bool IsPointInside(const Vec3& InPoint) const;
		bool IsAABBInside(const AABB& InOther) const;

		void Offset(const Vec3& InOffset);
	};

}
