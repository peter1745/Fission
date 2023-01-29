#include "FissionPCH.hpp"
#include "AABB.hpp"

namespace Fission::Math {

	bool AABB::IsPointInside(const FVec3& InPoint) const
	{
		if (InPoint.X < MinBound.X || InPoint.Y < MinBound.Y || InPoint.Z < MinBound.Z)
		{
			return false;
		}

		if (InPoint.X > MaxBound.X || InPoint.Y > MaxBound.Y || InPoint.Z > MaxBound.Z)
		{
			return false;
		}

		return true;
	}

	bool AABB::IsAABBInside(const AABB& InOther) const
	{
		if (MinBound.X > InOther.MaxBound.X || MinBound.Y > InOther.MaxBound.Y || MinBound.Z > InOther.MaxBound.Z)
		{
			return false;
		}

		if (MaxBound.X < InOther.MinBound.X || MaxBound.Y < InOther.MinBound.Y || MaxBound.Z < InOther.MinBound.Z)
		{
			return false;
		}

		return true;
	}

	void AABB::Offset(const FVec3& InOffset)
	{
		MinBound += InOffset;
		MaxBound += InOffset;
	}

}
