#pragma once

#include "FVec3.hpp"

namespace Fission::Math {

	struct AABB
	{
		FVec3 MinBound;
		FVec3 MaxBound;

		bool IsPointInside(const FVec3& InPoint) const;
		bool IsAABBInside(const AABB& InOther) const;

		void Offset(const FVec3& InOffset);
	};

}
