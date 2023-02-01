#include "FissionPCH.hpp"
#include "Math.hpp"

namespace Fission::Math {

	float Radians(float InDegrees)
	{
		return InDegrees * 0.01745329251994329576923690768489f;
	}

	float Clamp(float InValue, float InMin, float InMax)
	{
		if (InValue < InMin)
			return InMin;

		if (InValue > InMax)
			return InMax;

		return InValue;
	}

	bool IsNearZero(float InValue, float InEpsilon /*= 0.0005f*/)
	{
		return fabs(InValue) < InEpsilon;
	}

}
