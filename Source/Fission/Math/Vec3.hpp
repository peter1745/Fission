#pragma once

#include "Fission/Core/Core.hpp"

namespace Fission::Math {

	struct alignas(16) Vec3
	{
#ifdef FISSION_USE_SSE
		using Type = __m128;
#else
		using Type = struct { float Data[4]; };
#endif

		union
		{
			Type Value;
			float Floats[4];
		};

		Vec3() = default;
		Vec3(const Vec3&) = default;

		Vec3(float InScalar);
		Vec3(float InX, float InY, float InZ);
		Vec3(Type InValue)
		    : Value(InValue) {}

		Vec3 operator+(const Vec3& InOther) const;
		Vec3 operator+(float InValue) const;

		Vec3 operator-(const Vec3& InOther) const;
		Vec3 operator-(float InValue) const;

		Vec3 operator/(const Vec3& InOther) const;
		Vec3 operator/(float InValue) const;

		Vec3 operator*(const Vec3& InOther) const;
		Vec3 operator*(float InValue) const;

		Vec3& operator+=(const Vec3& InOther);
		Vec3& operator+=(float InValue);

		Vec3& operator-=(const Vec3& InOther);
		Vec3& operator-=(float InValue);
		
		Vec3& operator/=(const Vec3& InOther);
		Vec3& operator/=(float InValue);

		Vec3& operator*=(const Vec3& InOther);
		Vec3& operator*=(float InValue);

		Vec3 operator-() const;

#ifdef FISSION_USE_SSE
		float GetX() const { return _mm_cvtss_f32(Value); }
		float GetY() const { return Floats[1]; }
		float GetZ() const { return Floats[2]; }
#else
		float GetX() const { return Floats[0]; }
		float GetY() const { return Floats[1]; }
		float GetZ() const { return Floats[2]; }
#endif

		float Dot(const Vec3& InOther) const;

		float LengthSq() const { return Dot(*this); }
		float Length() const;

		float operator[](size_t InIdx) const { return Floats[InIdx]; }

		static Vec3 Min(const Vec3& InVec0, const Vec3& InVec1);
		static Vec3 Max(const Vec3& InVec0, const Vec3& InVec1);

		static Vec3 Zero();

	};

}
