#include "FissionPCH.hpp"
#include "AABB.hpp"

namespace Fission::Math {

	bool AABB::IsPointInside(const Vec3& InPoint) const
	{
		if (InPoint.GetX() < MinBound.GetX() || InPoint.GetY() < MinBound.GetY() || InPoint.GetZ() < MinBound.GetZ())
		{
			return false;
		}

		if (InPoint.GetX() > MaxBound.GetX() || InPoint.GetY() > MaxBound.GetY() || InPoint.GetZ() > MaxBound.GetZ())
		{
			return false;
		}

		return true;
	}

	bool AABB::IsAABBInside(const AABB& InOther) const
	{
		if (MinBound.GetX() > InOther.MaxBound.GetX() || MinBound.GetY() > InOther.MaxBound.GetY() || MinBound.GetZ() > InOther.MaxBound.GetZ())
		{
			return false;
		}

		if (MaxBound.GetX() < InOther.MinBound.GetX() || MaxBound.GetY() < InOther.MinBound.GetY() || MaxBound.GetZ() < InOther.MinBound.GetZ())
		{
			return false;
		}

		return true;
	}

	void AABB::Offset(const Vec3& InOffset)
	{
		MinBound += InOffset;
		MaxBound += InOffset;
	}

}
