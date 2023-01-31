#pragma once

#include "FVec3.hpp"
#include "FVec4.hpp"

namespace Fission::Math {

	struct FQuat
	{
		FVec4 Value;

		FQuat()
		    : Value(0.0f, 0.0f, 0.0f, 1.0) {}
		FQuat(const FQuat&) = default;
		FQuat(float InX, float InY, float InZ, float InW);
		FQuat(const FVec4& InValue);
		FQuat(const FVec3& InAxis, float InAngle);

		void operator+=(const FQuat& InOther) { Value += InOther.Value; }
		void operator-=(const FQuat& InOther) { Value -= InOther.Value; }
		void operator*=(float InValue) { Value *= InValue; }
		void operator/=(float InValue) { Value /= InValue; }
		FQuat operator-() const { return FQuat(-Value); }
		FQuat operator+(const FQuat& InOther) const { return FQuat(Value + InOther.Value); }
		FQuat operator-(const FQuat& InOther) const { return FQuat(Value - InOther.Value); }
		FQuat operator*(const FQuat& InOther) const;
		FQuat operator*(float InValue) const { return FQuat(Value * InValue); }
		inline friend FQuat operator*(float InValue, const FQuat& InOther) { return FQuat(InOther.Value * InValue); }
		FQuat operator/(float InValue) const { return FQuat(Value / InValue); }

		/// Rotate a vector by this quaternion
		FVec3 operator*(const FVec3& InValue) const;

		FQuat QProduct(const FQuat& InOther) const;

		FQuat Conjugated() const;

		void Normalize() { Value.Normalize(); }
		FQuat Normalized() const { return FQuat(Value.Normalized()); }

		FVec3 GetEulerAngles() const;
	};

}
