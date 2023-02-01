#pragma once

namespace Fission::Math {

	static constexpr float PI = 3.14159265358979323846f;

	float Radians(float InDegrees);
	float Clamp(float InValue, float InMin, float InMax);

	bool IsNearZero(float InValue, float InEpsilon = 0.0005f);

}
