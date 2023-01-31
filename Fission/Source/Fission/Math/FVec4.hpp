#pragma once

#include "FVec3.hpp"

namespace Fission::Math {

	struct FVec4
	{
		union
		{
			struct { float Values[4]; };
			struct { float X, Y, Z, W; };
		};

		FVec4() = default;
		FVec4(const FVec4&) = default;

		FVec4(float InScalar);
		FVec4(float InX, float InY, float InZ, float InW);
		FVec4(const FVec3& InXYZ, float InW);

		FVec4& operator+=(const FVec4& InOther);
		FVec4& operator+=(float InValue);
		FVec4& operator-=(const FVec4& InOther);
		FVec4& operator-=(float InValue);
		FVec4& operator/=(const FVec4& InOther);
		FVec4& operator/=(float InValue);
		FVec4& operator*=(const FVec4& InOther);
		FVec4& operator*=(float InValue);

		FVec4 operator+(const FVec4& InOther) const;
		FVec4 operator+(float InValue) const;
		FVec4 operator-(const FVec4& InOther) const;
		FVec4 operator-(float InValue) const;
		FVec4 operator/(const FVec4& InOther) const;
		FVec4 operator/(float InValue) const;
		FVec4 operator*(const FVec4& InOther) const;
		FVec4 operator*(float InValue) const;

		FVec4 operator-() const;

		float Dot(const FVec4& InOther) const;

		float LengthSq() const;
		float Length() const;

		void Normalize();
		FVec4 Normalized() const;

		float& operator[](size_t InIndex) { return Values[InIndex]; }
		float operator[](size_t InIndex) const;

		static FVec4 Max(const FVec4& InVec0, const FVec4& InVec1);
		static FVec4 Min(const FVec4& InVec0, const FVec4& InVec1);
		static FVec4 Zero();

	};

}
