#pragma once

#include "FVec3.hpp"
#include "FQuat.hpp"

#include <vector>

namespace Fission::Math {

	struct AABB
	{
		FVec3 MinBound;
		FVec3 MaxBound;

		bool IsPointInside(const FVec3& InPoint) const;
		bool IsAABBInside(const AABB& InOther) const;

		void Offset(const FVec3& InOffset);

		void Expand(const FVec3& InAmount);
		void Expand(const std::vector<FVec3>& InPoints);
	};

}
