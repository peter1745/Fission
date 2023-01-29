#pragma once

namespace Fission::Math {

	struct FVec4;

	struct FVec3
	{
		union
		{
			struct { float Values[3]; };
			struct { float X, Y, Z; };
		};

		FVec3() = default;
		FVec3(const FVec3&) = default;

		FVec3(float InScalar);
		FVec3(float InX, float InY, float InZ);
		FVec3(const FVec4& InOther);

		FVec3& operator+=(const FVec3& InOther);
		FVec3& operator+=(float InValue);
		FVec3& operator-=(const FVec3& InOther);
		FVec3& operator-=(float InValue);
		FVec3& operator/=(const FVec3& InOther);
		FVec3& operator/=(float InValue);
		FVec3& operator*=(const FVec3& InOther);
		FVec3& operator*=(float InValue);

		FVec3 operator+(const FVec3& InOther) const;
		FVec3 operator+(float InValue) const;
		FVec3 operator-(const FVec3& InOther) const;
		FVec3 operator-(float InValue) const;
		FVec3 operator/(const FVec3& InOther) const;
		FVec3 operator/(float InValue) const;
		FVec3 operator*(const FVec3& InOther) const;
		FVec3 operator*(float InValue) const;

		FVec3 operator-() const;

		float Dot(const FVec3& InOther) const;
		FVec3 Cross(const FVec3& InOther) const;

		float LengthSq() const;
		float Length() const;

		void Normalize();
		FVec3 Normalized() const;

		float& operator[](size_t InIndex);
		float operator[](size_t InIndex) const;

		static FVec3 Max(const FVec3& InVec0, const FVec3& InVec1);
		static FVec3 Min(const FVec3& InVec0, const FVec3& InVec1);
		static FVec3 Zero();

	};

}
